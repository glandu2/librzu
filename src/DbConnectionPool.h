#ifndef DBCONNECTIONPOOL_H
#define DBCONNECTIONPOOL_H

#include "Object.h"
#include "uv.h"
#include <list>
#include <string>
#include <sqlext.h>
#include "Log.h"
#include <string.h>

class DbConnection;

class RAPPELZLIB_EXTERN DbConnectionPool : public Object
{
	DECLARE_CLASSNAME(DbConnectionPool, 0)
public:
	DbConnectionPool();
	~DbConnectionPool();

	DbConnection* getConnection(const char* connectionString, std::string wantedQuery = std::string());
	DbConnection* addConnection(const char* connectionString, bool createLocked);
	void closeConnection(DbConnection* dbConnection);
	void* getHenv() { return henv; }

private:
	std::list<DbConnection*> openedConnections;
	uv_mutex_t listLock;
	void* henv;
};

class RAPPELZLIB_EXTERN DbConnection : public Object
{
	DECLARE_CLASS(DbConnection)
public:
	DbConnection(DbConnectionPool* conPool, void *hdbc, void *hstmt) : conPool(conPool), hdbc(hdbc), hstmt(hstmt) { uv_mutex_init(&lock); }
	virtual ~DbConnection() { SQLFreeHandle(SQL_HANDLE_STMT, hstmt); SQLDisconnect(hdbc); SQLFreeHandle(SQL_HANDLE_DBC, hdbc); uv_mutex_destroy(&lock); }

	bool trylock() { return uv_mutex_trylock(&lock) == 0; }
	void release() { SQLFreeStmt(hstmt, SQL_CLOSE); SQLFreeStmt(hstmt, SQL_RESET_PARAMS); uv_mutex_unlock(&lock); }
	void releaseWithError();

	bool bindParameter(SQLUSMALLINT       ipar,
					  SQLSMALLINT        fParamType,
					  SQLSMALLINT        fCType,
					  SQLSMALLINT        fSqlType,
					  SQLULEN            cbColDef,
					  SQLSMALLINT        ibScale,
					  SQLPOINTER         rgbValue,
					  SQLLEN             cbValueMax,
					  SQLLEN 		      *pcbValue)
	{
		return checkResult(SQLBindParameter(hstmt, ipar, fParamType, fCType, fSqlType, cbColDef, ibScale, rgbValue, cbValueMax, pcbValue), "SQLBindParameter");
	}

	bool execute(const char* query) {
		if(strcmp(lastQuery.c_str(), query)) {
			bool result = checkResult(SQLPrepare(hstmt, (SQLCHAR*)query, SQL_NTS), "SQLPrepare");
			if(!result)
				return false;
			lastQuery = query;
			debug("Cached DB query: %s\n", query);
		}
		return checkResult(SQLExecute(hstmt), "SQLExecute");
	}

	bool fetch() {
		return checkResult(SQLFetch(hstmt), "SQLFetch");
	}

	int getColumnNum(bool *ok) {
		SQLSMALLINT colCount = 0;
		bool isOk = checkResult(SQLNumResultCols(hstmt, &colCount), "SQLNumResultCols");

		if(ok != nullptr)
			*ok = isOk;

		return colCount;
	}

	bool getData(SQLUSMALLINT ColumnNumber,
				SQLSMALLINT TargetType,
				SQLPOINTER TargetValue,
				SQLLEN BufferLength,
				SQLLEN *StrLen_or_Ind)
	{
		return checkResult(SQLGetData(hstmt, ColumnNumber, TargetType, TargetValue, BufferLength, StrLen_or_Ind), "SQLGetData");
	}

	bool getColumnName(SQLUSMALLINT ColumnNumber,
					   char* ColumnName,
					   SQLSMALLINT BufferLength)
	{

		return checkResult(SQLColAttribute(hstmt, ColumnNumber, SQL_DESC_LABEL, ColumnName, BufferLength, NULL, NULL), "SQLColAttribute");
	}

	const std::string& getCachedQuery() { return lastQuery; }

protected:
	bool checkResult(SQLRETURN result, const char* function);

private:
	uv_mutex_t lock;
	DbConnectionPool* conPool;
	std::string lastQuery;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;
};


#endif // DBCONNECTIONPOOL_H
