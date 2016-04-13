#ifndef DBSESSIONQUERYJOB_H
#define DBSESSIONQUERYJOB_H

#include "DbQueryJob.h"

class DbQueryJobRef;

class IDbQueryJobCallback {
public:
	virtual void setDbQueryJobRef(DbQueryJobRef* dbQueryJobRef) = 0;
	virtual void cancel() = 0;
};

template<class DbMappingClass, class Session, class DbJobClass>
class DbQueryJobCallback : public DbQueryJob<DbMappingClass>, public IDbQueryJobCallback
{
public:
	typedef void (Session::*DbCallback)(DbJobClass* query);
public:
	DbQueryJobCallback(Session* session, DbCallback callback) : session(session), callback(callback), dbQueryJobRef(nullptr) {}

	void setDbQueryJobRef(DbQueryJobRef* dbQueryJobRef) { this->dbQueryJobRef = dbQueryJobRef; }
	DbQueryJobRef* getDbQueryJobRef() { return dbQueryJobRef; }

	void cancel();

protected:
	void onDone(IDbQueryJob::Status status);

private:
	Session* session;
	DbCallback callback;
	DbQueryJobRef* dbQueryJobRef;
};


class RZU_EXTERN DbQueryJobRef
{
public:
	DbQueryJobRef() : deleting(false) {}
	DbQueryJobRef(DbQueryJobRef&& other);

	~DbQueryJobRef();

	template<class DbMappingClass, class DbJobClass, class Session>
	void executeDbQuery(Session* session, typename DbJobClass::DbCallback callback, const typename DbMappingClass::Input& input) {
		auto query = new DbJobClass(session, callback);
		query->setDbQueryJobRef(this);
		dbQueryJobs.push_back(query);
		query->execute(&input, 1);
	}

	template<class DbMappingClass, class Session>
	void executeDbQuery(Session* session, typename DbQueryJobCallback<DbMappingClass, Session, DbQueryJob<DbMappingClass> >::DbCallback callback, const typename DbMappingClass::Input& input) {
		executeDbQuery<DbMappingClass, DbQueryJobCallback<DbMappingClass, Session, DbQueryJob<DbMappingClass> >, Session>(session, callback, input);
	}

	void onQueryDone(IDbQueryJobCallback* query);

	bool inProgress() { return !dbQueryJobs.empty(); }

private:
	std::vector<IDbQueryJobCallback*> dbQueryJobs;
	bool deleting;

	DbQueryJobRef(const DbQueryJobRef&);
	DbQueryJobRef& operator =(const DbQueryJobRef& other);
};

template<class DbMappingClass, class Session, class DbJobClass>
void DbQueryJobCallback<DbMappingClass, Session, DbJobClass>::cancel() {
	session = nullptr;
	if(dbQueryJobRef) {
		dbQueryJobRef->onQueryDone(this);
		dbQueryJobRef = nullptr;
	}
	DbQueryJob<DbMappingClass>::cancel();
}

template<class DbMappingClass, class Session, class DbJobClass>
void DbQueryJobCallback<DbMappingClass, Session, DbJobClass>::onDone(IDbQueryJob::Status status) {
	if(dbQueryJobRef) {
		dbQueryJobRef->onQueryDone(this);

		if(status != IDbQueryJob::S_Canceled && session && callback)
			(session->*callback)(static_cast<DbJobClass*>(this));

		dbQueryJobRef = nullptr;
	}
}

#endif // DBSESSIONQUERYJOB_H
