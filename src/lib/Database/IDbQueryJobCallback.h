#pragma once

class DbQueryJobRef;

class IDbQueryJobCallback {
public:
	virtual void setDbQueryJobRef(DbQueryJobRef* dbQueryJobRef) = 0;
	virtual void cancel() = 0;
};

