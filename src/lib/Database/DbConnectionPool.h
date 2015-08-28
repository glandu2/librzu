#ifndef DBCONNECTIONPOOL_H
#define DBCONNECTIONPOOL_H

#include "Core/Object.h"
#include "uv.h"
#include <list>
#include <string>

class DbConnection;

class RZU_EXTERN DbConnectionPool : public Object
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

	static bool checkSqlResult(int result, const char* function, void* henv, void* hdbc, void* hstmt);

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
