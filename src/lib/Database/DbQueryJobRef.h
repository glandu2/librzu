#pragma once

#include "DbQueryJobCallback.h"
#include "DbQueryJobLambda.h"

template<class DbMappingClass> class DbQueryJob;

class RZU_EXTERN DbQueryJobRef {
public:
	DbQueryJobRef() : deleting(false) {}
	DbQueryJobRef(DbQueryJobRef&& other);

	~DbQueryJobRef();

	template<class DbMappingClass, class DbJobClass, class Session>
	void executeDbQuery(Session* session,
	                    typename DbJobClass::DbCallback callback,
	                    const typename DbMappingClass::Input& input) {
		auto query = new DbJobClass(session, callback);
		query->setDbQueryJobRef(this);
		dbQueryJobs.push_back(query);
		query->execute(&input, 1);
	}

	template<class DbMappingClass, class DbJobClass, class Session>
	void executeDbQuery(Session* session,
	                    typename DbJobClass::DbCallback callback,
	                    const std::vector<typename DbMappingClass::Input>& inputs) {
		auto query = new DbJobClass(session, callback);
		query->setDbQueryJobRef(this);
		dbQueryJobs.push_back(query);
		query->execute(inputs.data(), inputs.size());
	}

	template<class DbMappingClass, class Session>
	void executeDbQuery(
	    Session* session,
	    typename DbQueryJobCallback<DbMappingClass, Session, DbQueryJob<DbMappingClass>>::DbCallback callback,
	    const typename DbMappingClass::Input& input) {
		executeDbQuery<DbMappingClass,
		               DbQueryJobCallback<DbMappingClass, Session, DbQueryJob<DbMappingClass>>,
		               Session>(session, callback, input);
	}

	template<class DbMappingClass, class Session>
	void executeDbQuery(
	    Session* session,
	    typename DbQueryJobCallback<DbMappingClass, Session, DbQueryJob<DbMappingClass>>::DbCallback callback,
	    const std::vector<typename DbMappingClass::Input>& inputs) {
		executeDbQuery<DbMappingClass,
		               DbQueryJobCallback<DbMappingClass, Session, DbQueryJob<DbMappingClass>>,
		               Session>(session, callback, inputs);
	}

	template<class DbMappingClass, class Lambda>
	void executeDbQuery(Lambda callback, const std::vector<typename DbMappingClass::Input>& inputs) {
		auto query = new DbQueryJobLambda<DbMappingClass, Lambda>(callback);
		query->setDbQueryJobRef(this);
		dbQueryJobs.push_back(query);
		query->execute(inputs.data(), inputs.size());
	}

	void onQueryDone(IDbQueryJobCallback* query);

	bool inProgress() { return !dbQueryJobs.empty(); }

private:
	std::vector<IDbQueryJobCallback*> dbQueryJobs;
	bool deleting;

	DbQueryJobRef(const DbQueryJobRef&);
	DbQueryJobRef& operator=(const DbQueryJobRef& other);
};

