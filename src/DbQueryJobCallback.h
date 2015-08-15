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

	void cancel() {
		session = nullptr;
		if(dbQueryJobRef) {
			dbQueryJobRef->onQueryDone(this);
			dbQueryJobRef = nullptr;
		}
		DbQueryJob::cancel();
	}

protected:
	void onDone(Status status) {
		if(dbQueryJobRef)
			dbQueryJobRef->onQueryDone(this);

		if(status != S_Canceled && dbQueryJobRef && session && callback)
			(session->*callback)(static_cast<DbJobClass*>(this));

		if(dbQueryJobRef)
			dbQueryJobRef = nullptr;
	}

private:
	Session* session;
	DbCallback callback;
	DbQueryJobRef* dbQueryJobRef;
};


class DbQueryJobRef
{
public:
	DbQueryJobRef() : deleting(false) {}
	DbQueryJobRef(DbQueryJobRef&& other) {
		dbQueryJobs.swap(other.dbQueryJobs);

		auto it = dbQueryJobs.begin();
		auto itEnd = dbQueryJobs.end();
		for(; it != itEnd; ++it) {
			(*it)->setDbQueryJobRef(this);
		}
	}

	~DbQueryJobRef() {
		deleting = true;

		auto it = dbQueryJobs.begin();
		auto itEnd = dbQueryJobs.end();
		for(; it != itEnd; ++it) {
			(*it)->cancel();
		}
	}

	template<class DbMappingClass, class DbJobClass, class Session>
	void executeDbQuery(Session* session, typename DbJobClass::DbCallback callback, const typename DbMappingClass::Input& input) {
		auto query = new DbJobClass(session, callback);
		query->setDbQueryJobRef(this);
		dbQueryJobs.push_back(query);
		query->execute(input);
	}

	template<class DbMappingClass, class Session>
	void executeDbQuery(Session* session, typename DbQueryJobCallback<DbMappingClass, Session, DbQueryJob<DbMappingClass> >::DbCallback callback, const typename DbMappingClass::Input& input) {
		executeDbQuery<DbMappingClass, DbQueryJobCallback<DbMappingClass, Session, DbQueryJob<DbMappingClass> >, Session>(session, callback, input);
	}

	void onQueryDone(IDbQueryJobCallback* query) {
		if(!deleting)
			dbQueryJobs.remove(query);
	}

	bool inProgress() { return !dbQueryJobs.empty(); }

private:
	std::list<IDbQueryJobCallback*> dbQueryJobs;
	bool deleting;

	DbQueryJobRef(const DbQueryJobRef&);
	DbQueryJobRef& operator =(const DbQueryJobRef& other);
};

#endif // DBSESSIONQUERYJOB_H
