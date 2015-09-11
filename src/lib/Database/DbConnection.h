#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include "Core/Object.h"
#include "uv.h"
#include <string>
#include <sqlext.h>
#include <string.h>
#include <assert.h>

class DbConnectionPool;

class RZU_EXTERN DbConnection : public Object
{
	DECLARE_CLASS(DbConnection)
public:
	DbConnection(DbConnectionPool* conPool, void *hdbc, void *hstmt);
	virtual ~DbConnection();

	bool trylock() {
		bool locked = false;
		uv_mutex_lock(&usedLock);
		if(isUsed == false) {
			locked = isUsed = true;
		}
		uv_mutex_unlock(&usedLock);
		return locked;

//		return uv_mutex_trylock(&lock) == 0;
	}

	void release() {
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
		uv_mutex_lock(&usedLock);
		isUsed = false;
		uv_mutex_unlock(&usedLock);
//		uv_mutex_unlock(&lock);
	}

	//Will delete this
	void releaseWithError() { releaseAndClose(); }
	void releaseAndClose();

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
		assert(isUsed);
		return checkResult(SQLBindParameter(hstmt, ipar, fParamType, fCType, fSqlType, cbColDef, ibScale, rgbValue, cbValueMax, pcbValue), "SQLBindParameter");
	}

	bool execute(const char* query) {
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

	bool fetch() {
		assert(isUsed);
		return checkResult(SQLFetch(hstmt), "SQLFetch");
	}

	int getColumnNum(bool *ok) {
		SQLSMALLINT colCount = 0;
		assert(isUsed);
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
		assert(isUsed);
		return checkResult(SQLGetData(hstmt, ColumnNumber, TargetType, TargetValue, BufferLength, StrLen_or_Ind), "SQLGetData");
	}

	bool getColumnName(SQLUSMALLINT ColumnNumber,
					   char* ColumnName,
					   SQLSMALLINT BufferLength)
	{
		assert(isUsed);
		return checkResult(SQLColAttribute(hstmt, ColumnNumber, SQL_DESC_LABEL, ColumnName, BufferLength, NULL, NULL), "SQLColAttribute");
	}

	const std::string& getCachedQuery() { return lastQuery; }

protected:
	bool checkResult(SQLRETURN result, const char* function);

private:
//	uv_mutex_t lock;
	DbConnectionPool* conPool;
	std::string lastQuery;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;

	uv_mutex_t usedLock;
	bool isUsed; //workaround trylock in valgrind ...
};


#endif // DBCONNECTION_H
