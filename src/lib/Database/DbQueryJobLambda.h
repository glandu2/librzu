#ifndef DBQUERYJOBLAMBDA_H
#define DBQUERYJOBLAMBDA_H

#include "DbQueryJob.h"

class DbQueryJobLambdaBase {
public:
	template<class DbMappingClass, typename LambdaCallback>
	static void execute(const std::vector<typename DbQueryJob<DbMappingClass>::InputType>& inputs,
	                    LambdaCallback callback);
};

template<class DbMappingClass, typename LambdaCallback>
class DbQueryJobLambda : public DbQueryJob<DbMappingClass>, public DbQueryJobLambdaBase {
public:
	DbQueryJobLambda(LambdaCallback callback) : callback(callback) {}

protected:
	void onDone(IDbQueryJob::Status status) { callback(this, status); }

private:
	LambdaCallback callback;
};

template<class DbMappingClass, typename LambdaCallback>
void DbQueryJobLambdaBase::execute(const std::vector<typename DbQueryJob<DbMappingClass>::InputType>& inputs,
                                   LambdaCallback callback) {
	auto query = new DbQueryJobLambda<DbMappingClass, LambdaCallback>(callback);
	query->DbQueryJob<DbMappingClass>::execute(inputs.data(), inputs.size());
}

#endif  // DBQUERYJOBCALLBACK_H
