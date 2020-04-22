#pragma once

#include "DbQueryJob.h"
#include "IDbQueryJobCallback.h"

class DbQueryJobRef;

class RZU_EXTERN DbQueryJobCallbackBase {
public:
	void notifyDone(DbQueryJobRef* dbQueryJobRef, IDbQueryJobCallback* query);
};

template<class DbMappingClass, class Session, class DbJobClass>
class DbQueryJobCallback : public DbQueryJob<DbMappingClass>,
                           public IDbQueryJobCallback,
                           public DbQueryJobCallbackBase {
public:
	typedef void (Session::*DbCallback)(DbJobClass* query);

public:
	DbQueryJobCallback(Session* session, DbCallback callback)
	    : session(session), callback(callback), dbQueryJobRef(nullptr) {}

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

#include "DbQueryJobCallback.inl"

