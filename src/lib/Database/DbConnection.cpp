#include "DbConnection.h"
#include "DbConnectionPool.h"

DbConnection::DbConnection(DbConnectionPool* conPool, void *hdbc, void *hstmt) : conPool(conPool), hdbc(hdbc), hstmt(hstmt)
, isUsed(false)
{
//	uv_mutex_init(&lock);
	uv_mutex_init(&usedLock);
}

DbConnection::~DbConnection() {
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
//	uv_mutex_destroy(&lock);
	uv_mutex_destroy(&usedLock);
}

bool DbConnection::trylock() {
	bool locked = false;
	uv_mutex_lock(&usedLock);
	if(isUsed == false) {
		locked = isUsed = true;
	}
	uv_mutex_unlock(&usedLock);
	return locked;

//	return uv_mutex_trylock(&lock) == 0;
}

void DbConnection::release() {
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
	uv_mutex_lock(&usedLock);
	isUsed = false;
	uv_mutex_unlock(&usedLock);
//	uv_mutex_unlock(&lock);
}

void DbConnection::releaseWithError() {
	releaseAndClose();
}

void DbConnection::releaseAndClose() {
	conPool->removeConnection(this);

	release();
	delete this;
}

void DbConnection::setAutoCommit(bool enable)
{
	checkResult(SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT,(SQLPOINTER) (enable ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF), SQL_IS_UINTEGER), "SQL_ATTR_AUTOCOMMIT");
}

void DbConnection::endTransaction(bool commit)
{
	checkResult(SQLEndTran(SQL_HANDLE_DBC, hdbc, commit ? SQL_COMMIT : SQL_ROLLBACK), commit ? "commit" : "rollback");
}

bool DbConnection::bindParameter(SQLUSMALLINT ipar, SQLSMALLINT fParamType, SQLSMALLINT fCType, SQLSMALLINT fSqlType, SQLULEN cbColDef, SQLSMALLINT ibScale, SQLPOINTER rgbValue, SQLLEN cbValueMax, SQLLEN *pcbValue) {
	assert(isUsed);
	return checkResult(SQLBindParameter(hstmt, ipar, fParamType, fCType, fSqlType, cbColDef, ibScale, rgbValue, cbValueMax, pcbValue), "SQLBindParameter");
}

bool DbConnection::execute(const char *query) {
	assert(isUsed);
	if(strcmp(lastQuery.c_str(), query)) {
		bool result = checkResult(SQLPrepare(hstmt, (SQLCHAR*)query, SQL_NTS), "SQLPrepare");
		if(!result)
			return false;
		lastQuery = query;
		log(LL_Debug, "Cached DB query: %s\n", query);
	}
	return checkResult(SQLExecute(hstmt), "SQLExecute");
}

bool DbConnection::fetch() {
	assert(isUsed);
	return checkResult(SQLFetch(hstmt), "SQLFetch");
}

int DbConnection::getColumnNum(bool *ok) {
	SQLSMALLINT colCount = 0;
	assert(isUsed);
	bool isOk = checkResult(SQLNumResultCols(hstmt, &colCount), "SQLNumResultCols");

	if(ok != nullptr)
		*ok = isOk;

	return colCount;
}

bool DbConnection::getData(SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, SQLPOINTER TargetValue, SQLLEN BufferLength, SQLLEN *StrLen_or_Ind, bool silentInfo, SQLRETURN* rawResultCode) {
	assert(isUsed);
	SQLRETURN result = SQLGetData(hstmt, ColumnNumber, TargetType, TargetValue, BufferLength, StrLen_or_Ind);
	if(rawResultCode)
		*rawResultCode = result;
	return checkResult(result, "SQLGetData", silentInfo);
}

bool DbConnection::getColumnName(SQLUSMALLINT ColumnNumber, char *ColumnName, SQLSMALLINT BufferLength) {
	assert(isUsed);
	return checkResult(SQLColAttribute(hstmt, ColumnNumber, SQL_DESC_LABEL, ColumnName, BufferLength, NULL, NULL), "SQLColAttribute");
}

bool DbConnection::checkResult(SQLRETURN result, const char* function, bool silentInfo) {
	return DbConnectionPool::checkSqlResult(result, function, conPool->getHenv(), hdbc, hstmt, silentInfo);
}
