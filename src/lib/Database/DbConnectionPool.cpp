#include "DbConnectionPool.h"
#include "Console/ConsoleCommands.h"
#include "DbConnection.h"
#include <list>
#include <sqlext.h>
#include <stdlib.h>

static void outputError(Object::Level errorLevel, SQLHANDLE handle, SQLSMALLINT type, bool discard);

DbConnectionPool* DbConnectionPool::instance = nullptr;

DbConnectionPool::DbConnectionPool() {
	SQLRETURN result;

	if(instance == nullptr) {
		instance = this;
		ConsoleCommands::get()->addCommand(
		    "db.close",
		    "closedb",
		    0,
		    0,
		    &commandCloseDbConnections,
		    "Close all idle database connections (use this to bring a database offline)");
	}

	uv_mutex_init(&listLock);

	result = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	if(!SQL_SUCCEEDED(result)) {
		log(LL_Fatal, "Can\t allocate ODBC ENV handle\n");
		abort();
	}
	result = checkSqlResult(SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER),
	                        "SQLSetEnvAttrODBC3",
	                        henv,
	                        nullptr,
	                        nullptr);
	if(!result)
		log(LL_Error, "Can\'t use ODBC 3\n");
}

DbConnectionPool::~DbConnectionPool() {
	std::unordered_multimap<std::string, DbConnection*>::iterator it, itEnd;

	log(LL_Info, "Closing DB connections pool\n");

	for(it = openedConnections.begin(), itEnd = openedConnections.end(); it != itEnd; ++it) {
		const std::pair<std::string, DbConnection*>& connection = *it;
		delete connection.second;
	}
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
	uv_mutex_destroy(&listLock);
}

bool DbConnectionPool::checkConnection(const char* connectionString) {
	DbConnection* dbConnection;

	log(LL_Info, "Checking connection to database\n");
	dbConnection = getConnection(connectionString, "");
	if(!dbConnection) {
		log(LL_Error, "Could not retrieve a DB connection from pool to \"%s\"\n", connectionString);
		return false;
	}
	dbConnection->releaseAndClose();
	log(LL_Info, "Connection ok\n");
	return true;
}

DbConnection* DbConnectionPool::getConnection(const char* connectionString, std::string wantedQuery) {
	DbConnection* dbConnection = nullptr;

	uv_mutex_lock(&listLock);
	auto range = openedConnections.equal_range(std::string(connectionString));
	auto it = range.first;
	auto itEnd = range.second;
	for(; it != itEnd; ++it) {
		const std::pair<std::string, DbConnection*>& data = *it;
		DbConnection* connection = data.second;
		if(connection->trylock()) {
			if(wantedQuery == std::string() || connection->getCachedQuery() == wantedQuery) {
				dbConnection = connection;
				break;
			} else {
				connection->release();
			}
		}
	}
	// If more than 8 connections are already opened, try to reuse one even if the cached query is not the same
	if(dbConnection == nullptr && openedConnections.size() >= 8) {
		it = range.first;
		itEnd = range.second;
		for(; it != itEnd; ++it) {
			const std::pair<std::string, DbConnection*>& data = *it;
			DbConnection* connection = data.second;
			if(connection->trylock()) {
				dbConnection = connection;
				break;
			}
		}
	}
	uv_mutex_unlock(&listLock);

	if(dbConnection)
		return dbConnection;
	else
		return addConnection(connectionString, true);
}

DbConnection* DbConnectionPool::addConnection(const char* connectionString, bool createLocked) {
	SQLRETURN result;
	SQLHDBC hdbc = nullptr;
	SQLHSTMT hstmt = nullptr;

	log(LL_Debug, "Connecting to database\n");

	result = checkSqlResult(SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc), "SQLAllocHandle", henv, nullptr, nullptr);
	if(!result)
		return nullptr;

	result = checkSqlResult(SQLDriverConnect(hdbc, nullptr, (UCHAR*) connectionString, SQL_NTS, nullptr, 0, nullptr, 0),
	                        "SQLDriverConnect",
	                        henv,
	                        hdbc,
	                        nullptr);
	if(!result) {
		log(LL_Error, "Failed to connect to %s\n", connectionString);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		return nullptr;
	}

	result = checkSqlResult(SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt), "SQLAllocHandle", henv, hdbc, nullptr);
	if(!result) {
		log(LL_Error, "Failed to alloc a resultset for %s\n", connectionString);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		return nullptr;
	}

	// 20 sec timeout
	checkSqlResult(SQLSetStmtAttr(hstmt, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER) 10, 0),
	               "SQLSetStmtAttrTimeout (error ignored)",
	               henv,
	               hdbc,
	               hstmt,
	               true);
	checkSqlResult(SQLSetConnectAttr(hdbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER) 10, 0),
	               "SQLSetConnectAttrTimeout (error ignored)",
	               henv,
	               hdbc,
	               hstmt,
	               true);

	DbConnection* dbConnection = new DbConnection(this, hdbc, hstmt);
	if(createLocked)
		dbConnection->trylock();

	uv_mutex_lock(&listLock);
	openedConnections.insert(std::make_pair(std::string(connectionString), dbConnection));
	uv_mutex_unlock(&listLock);

	return dbConnection;
}

void DbConnectionPool::removeConnection(DbConnection* dbConnection) {
	uv_mutex_lock(&listLock);
	std::unordered_multimap<std::string, DbConnection*>::iterator it, itEnd;
	for(it = openedConnections.begin(), itEnd = openedConnections.end(); it != itEnd; ++it) {
		const std::pair<std::string, DbConnection*>& data = *it;
		if(data.second == dbConnection) {
			openedConnections.erase(it);
			break;
		}
	}
	uv_mutex_unlock(&listLock);
}

int DbConnectionPool::closeAllConnections() {
	std::list<DbConnection*> connectionsToRemove;
	std::list<DbConnection*>::iterator itList, itListEnd;
	std::unordered_multimap<std::string, DbConnection*>::iterator it, itEnd;

	// Retrieve and lock all idle connections
	uv_mutex_lock(&listLock);
	for(it = openedConnections.begin(), itEnd = openedConnections.end(); it != itEnd; ++it) {
		const std::pair<std::string, DbConnection*>& data = *it;
		DbConnection* connection = data.second;
		if(connection->trylock()) {
			connectionsToRemove.push_back(connection);
		}
	}
	uv_mutex_unlock(&listLock);

	// Release and close them
	for(itList = connectionsToRemove.begin(), itListEnd = connectionsToRemove.end(); itList != itListEnd; ++itList) {
		DbConnection* connection = *itList;
		connection->releaseAndClose();
	}

	return (int) connectionsToRemove.size();
}

bool DbConnectionPool::checkSqlResult(
    int result, const char* function, void* henv, void* hdbc, void* hstmt, bool silentInfo) {
	if(result == SQL_SUCCESS_WITH_INFO) {
		if(!silentInfo)
			logStatic(LL_Debug, "ODBC", "%s: additional info:\n", function);
		if(hstmt)
			outputError(LL_Debug, hstmt, SQL_HANDLE_STMT, silentInfo);
		if(hdbc)
			outputError(LL_Debug, hdbc, SQL_HANDLE_DBC, silentInfo);
		if(henv)
			outputError(LL_Debug, henv, SQL_HANDLE_ENV, silentInfo);
	} else if(result == SQL_ERROR) {
		logStatic(LL_Error, "ODBC", "%s: error:\n", function);
		if(hstmt)
			outputError(LL_Error, hstmt, SQL_HANDLE_STMT, false);
		if(hdbc)
			outputError(LL_Error, hdbc, SQL_HANDLE_DBC, false);
		if(henv)
			outputError(LL_Error, henv, SQL_HANDLE_ENV, false);
	}

	return SQL_SUCCEEDED(result);
}

static void outputError(Object::Level errorLevel, SQLHANDLE handle, SQLSMALLINT type, bool discard) {
	SQLSMALLINT i = 0, len;
	SQLINTEGER native;
	SQLCHAR state[7];
	SQLCHAR text[256];
	SQLRETURN ret;

	do {
		ret = SQLGetDiagRec(type, handle, ++i, state, &native, text, sizeof(text), &len);
		if(!discard && SQL_SUCCEEDED(ret)) {
			Object::logStatic(errorLevel, "ODBC", "%s:%d:%ld:%s\n", state, i, (long) native, text);
		}
	} while(ret == SQL_SUCCESS);
}

void DbConnectionPool::commandCloseDbConnections(IWritableConsole* console, const std::vector<std::string>& args) {
	int connectionsClosed = getInstance()->closeAllConnections();
	console->writef("Closed %d DB connections\r\n", connectionsClosed);
}
