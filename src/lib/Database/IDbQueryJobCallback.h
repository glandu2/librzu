#ifndef IDBQUERYJOBCALLBACK_H
#define IDBQUERYJOBCALLBACK_H

class DbQueryJobRef;

class IDbQueryJobCallback {
public:
	virtual void setDbQueryJobRef(DbQueryJobRef* dbQueryJobRef) = 0;
	virtual void cancel() = 0;
};

#endif  // IDBQUERYJOBCALLBACK_H
