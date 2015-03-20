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

void DbConnection::releaseAndClose() {
	conPool->removeConnection(this);

	release();
	delete this;
}

bool DbConnection::checkResult(SQLRETURN result, const char* function) {
	return DbConnectionPool::checkSqlResult(result, function, conPool->getHenv(), hdbc, hstmt);
}
