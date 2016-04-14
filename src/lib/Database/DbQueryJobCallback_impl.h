#ifndef DBQUERYJOBCALLBACK_IMPL_H
#define DBQUERYJOBCALLBACK_IMPL_H

#include "DbQueryJob.h"
#include "DbQueryJobCallback.h"

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

#endif // DBQUERYJOBCALLBACK_IMPL_H
