#ifndef DBCONNECTIONPOOL_H
#define DBCONNECTIONPOOL_H

#include "Object.h"
#include "uv.h"
#include <list>
#include <string>
#include <sqlext.h>
#include "Log.h"

class DbConnection;

class RAPPELZLIB_EXTERN DbConnectionPool : public Object
{
	DECLARE_CLASSNAME(DbConnectionPool, 0)
public:
	DbConnectionPool();
	~DbConnectionPool();

	bool checkConnection(const char* connectionString);

	DbConnection* getConnection(const char* connectionString, std::string wantedQuery = std::string());
	DbConnection* addConnection(const char* connectionString, bool createLocked);
	int closeAllConnections();
	void* getHenv() { return henv; }

	static bool checkSqlResult(SQLRETURN result, const char* function, SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt);

	static DbConnectionPool* getInstance() { return instance; }

protected:
	void removeConnection(DbConnection* dbConnection);
	friend class DbConnection;

private:
	static DbConnectionPool *instance;
	std::list<DbConnection*> openedConnections;
	uv_mutex_t listLock;
	void* henv;
};

#endif // DBCONNECTIONPOOL_H
