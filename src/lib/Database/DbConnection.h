#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include "Core/Object.h"
#include "uv.h"
#include <sqlext.h>
#include <string>

class DbConnectionPool;

class RZU_EXTERN DbConnection : public Object {
	DECLARE_CLASS(DbConnection)
public:
	DbConnection(DbConnectionPool* conPool, void* hdbc, void* hstmt);
	virtual ~DbConnection();

	bool trylock();

	void release();

	// Will delete this
	void releaseWithError();
	void releaseAndClose();
	void setAutoCommit(bool enable);
	void endTransaction(bool commit);

	bool bindParameter(SQLUSMALLINT ipar,
	                   SQLSMALLINT fParamType,
	                   SQLSMALLINT fCType,
	                   SQLSMALLINT fSqlType,
	                   SQLULEN cbColDef,
	                   SQLSMALLINT ibScale,
	                   SQLPOINTER rgbValue,
	                   SQLLEN cbValueMax,
	                   SQLLEN* pcbValue);

	bool setAttribute(SQLINTEGER attribute, SQLPOINTER value, SQLINTEGER stringLength);

	bool execute(const char* query);
	bool fetch();
	bool closeCursor();
	int getColumnNum(bool* ok);

	bool getData(SQLUSMALLINT ColumnNumber,
	             SQLSMALLINT TargetType,
	             SQLPOINTER TargetValue,
	             SQLLEN BufferLength,
	             SQLLEN* StrLen_or_Ind,
	             bool silentInfo = false,
	             SQLRETURN* rawResultCode = nullptr);

	bool getColumnName(SQLUSMALLINT ColumnNumber, char* ColumnName, SQLSMALLINT BufferLength);

	const std::string& getCachedQuery() { return lastQuery; }

	bool checkResult(SQLRETURN result, const char* function, bool silentInfo = false);

private:
	//	uv_mutex_t lock;
	DbConnectionPool* conPool;
	std::string lastQuery;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;

	uv_mutex_t usedLock;
	bool isUsed;  // workaround trylock in valgrind ...
};

#endif  // DBCONNECTION_H
