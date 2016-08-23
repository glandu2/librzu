#include "DbQueryJobCallback.h"
#include "DbQueryJobRef.h"

void DbQueryJobCallbackBase::notifyDone(DbQueryJobRef* dbQueryJobRef, IDbQueryJobCallback* query)
{
	dbQueryJobRef->onQueryDone(query);
}
