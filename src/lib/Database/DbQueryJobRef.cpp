#include "DbQueryJobRef.h"
#include "IDbQueryJobCallback.h"
#include <algorithm>

DbQueryJobRef::DbQueryJobRef(DbQueryJobRef&& other) {
	dbQueryJobs.swap(other.dbQueryJobs);

	auto it = dbQueryJobs.begin();
	auto itEnd = dbQueryJobs.end();
	for(; it != itEnd; ++it) {
		(*it)->setDbQueryJobRef(this);
	}
}

DbQueryJobRef::~DbQueryJobRef() {
	deleting = true;

	auto it = dbQueryJobs.begin();
	auto itEnd = dbQueryJobs.end();
	for(; it != itEnd; ++it) {
		(*it)->cancel();
	}
}

void DbQueryJobRef::onQueryDone(IDbQueryJobCallback* query) {
	if(!deleting) {
		auto it = std::find(dbQueryJobs.begin(), dbQueryJobs.end(), query);
		if(it != dbQueryJobs.end())
			dbQueryJobs.erase(it);
	}
}
