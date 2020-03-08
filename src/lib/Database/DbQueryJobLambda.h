#ifndef DBQUERYJOBLAMBDA_H
#define DBQUERYJOBLAMBDA_H

#include "DbQueryJob.h"
#include "DbQueryJobCallback.h"
#include "IDbQueryJobCallback.h"

template<class DbMappingClass, typename LambdaCallback>
class DbQueryJobLambda : public DbQueryJob<DbMappingClass>, public IDbQueryJobCallback, public DbQueryJobCallbackBase {
public:
	DbQueryJobLambda(LambdaCallback callback) : callback(callback), isCanceled(false) {}

	virtual void setDbQueryJobRef(DbQueryJobRef* dbQueryJobRef) override { this->dbQueryJobRef = dbQueryJobRef; }
	virtual void cancel() override {
		isCanceled = true;
		if(dbQueryJobRef) {
			notifyDone(dbQueryJobRef, this);
			dbQueryJobRef = nullptr;
		}
		DbQueryJob<DbMappingClass>::cancel();
	}

protected:
	void onDone(IDbQueryJob::Status status) {
		if(!isCanceled)
			callback(this, status);
	}

private:
	LambdaCallback callback;
	bool isCanceled;
	DbQueryJobRef* dbQueryJobRef;
};

#endif  // DBQUERYJOBCALLBACK_H
