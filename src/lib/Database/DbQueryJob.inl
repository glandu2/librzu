#ifndef DBQUERYJOB_IMPL_H
#define DBQUERYJOB_IMPL_H

#include "DbQueryJob.h"
#include "Core/EventLoop.h"
#include <assert.h>

template<class DbMappingClass> template<typename FieldType>
void DbQueryJob<DbMappingClass>::addParam(const char* columnName, size_t memberOffset, SQLLEN InputType::*nullIndicator) {
	typedef typename std::remove_reference<FieldType>::type ValueFieldType;
	char configName[512];
	sprintf(configName, "sql.%s.param.%s", SQL_CONFIG_NAME, columnName);
	dbBinding->addParameter(DbQueryBinding::ParameterBinding(
							SQL_PARAM_INPUT,
							ConfigInfo::get()->createValue<cval>(configName, (int)dbBinding->getParameterCount() + 1, true),
							DbTypeBinding<ValueFieldType>::C_TYPE,
							DbTypeBinding<ValueFieldType>::SQL_TYPE,
							DbTypeBinding<ValueFieldType>::SQL_SIZE,
							DbTypeBinding<ValueFieldType>::SQL_PRECISION,
							IsStdString<ValueFieldType>::value,
							memberOffset,
							nullIndicator ? (size_t)(&(((InputType*)0)->*nullIndicator)) : (size_t)-1));
}

template<class DbMappingClass> template<typename FieldType>
void DbQueryJob<DbMappingClass>::addOutputParam(const char* columnName, size_t memberOffset, SQLLEN InputType::*nullIndicator) {
	typedef typename std::remove_reference<FieldType>::type ValueFieldType;
	char configName[512];
	sprintf(configName, "sql.%s.outparam.%s", SQL_CONFIG_NAME, columnName);
	dbBinding->addParameter(DbQueryBinding::ParameterBinding(
							SQL_PARAM_OUTPUT,
							ConfigInfo::get()->createValue<cval>(configName, (int)dbBinding->getParameterCount() + 1, true),
							DbTypeBinding<ValueFieldType>::C_TYPE,
							DbTypeBinding<ValueFieldType>::SQL_TYPE,
							DbTypeBinding<ValueFieldType>::SQL_SIZE,
							DbTypeBinding<ValueFieldType>::SQL_PRECISION,
							IsStdString<ValueFieldType>::value,
							memberOffset,
							nullIndicator ? (size_t)(&(((InputType*)0)->*nullIndicator)) : (size_t)-1));
}

template<class DbMappingClass> template<typename FieldType>
void DbQueryJob<DbMappingClass>::addColumn(const char* columnName, size_t memberOffset, SQLLEN size, bool OutputType::*nullIndicator) {
	typedef typename std::remove_reference<FieldType>::type ValueFieldType;
	char configName[512];
	sprintf(configName, "sql.%s.column.%s", SQL_CONFIG_NAME, columnName);
	dbBinding->addColumn(DbQueryBinding::ColumnBinding(
							 ConfigInfo::get()->createValue<cval>(configName, columnName, true),
							 DbTypeBinding<ValueFieldType>::C_TYPE,
							 IsStdString<ValueFieldType>::value,
							 memberOffset,
							 size,
							 nullIndicator ? (size_t)(&(((OutputType*)0)->*nullIndicator)) : (size_t)-1));
}


template<class DbMappingClass>
void DbQueryJob<DbMappingClass>::deinit() {
	DbQueryBinding* binding = dbBinding;
	dbBinding = nullptr;
	delete binding;
}

template<class DbMappingClass>
void DbQueryJob<DbMappingClass>::createBinding(DbConnectionPool *dbConnectionPool,
										  cval<std::string> &connectionString,
										  const char *query,
										  DbQueryBinding::ExecuteMode executeMode)
{
	char enableConfigName[512];
	char queryConfigName[512];
	sprintf(enableConfigName, "sql.%s.enable", SQL_CONFIG_NAME);
	sprintf(queryConfigName, "sql.%s.query", SQL_CONFIG_NAME);
	assert(dbBinding == nullptr);
	dbBinding = new DbQueryBinding(dbConnectionPool,
							  ConfigInfo::get()->createValue<cval>(enableConfigName, true, true),
							  connectionString,
							  ConfigInfo::get()->createValue<cval>(queryConfigName, query, true),
							  executeMode);
}

template<class DbMappingClass>
void DbQueryJob<DbMappingClass>::executeNoResult(const InputType &input) {
	auto query = new DbQueryJob;
	query->execute(&input, 1);
}

template<class DbMappingClass>
void DbQueryJob<DbMappingClass>::executeNoResult(const std::vector<InputType> &inputs) {
	auto query = new DbQueryJob;
	query->execute(inputs.data(), inputs.size());
}

template<class DbMappingClass>
bool DbQueryJob<DbMappingClass>::execute(const InputType* inputs, size_t number) {
	done = false;
	canceled = false;

	if(dbBinding == nullptr) {
		log(LL_Error, "DB binding was not initialized ! Canceling DB query\n");
		onDone(S_Canceled);
		return false;
	}

	this->input.reserve(number);
	for(size_t i = 0; i < number; i++)
		this->input.emplace_back(inputs[i]);
	req.data = this;
	uv_queue_work(EventLoop::getLoop(), &req, &onProcessStatic, &onDoneStatic);

	return true;
}

template<class DbMappingClass>
void DbQueryJob<DbMappingClass>::cancel() {
	canceled = true;
	uv_cancel((uv_req_t*)&req);
}

template<class DbMappingClass>
void DbQueryJob<DbMappingClass>::onProcessStatic(uv_work_t *req) {
	DbQueryJob* dbQueryJob = (DbQueryJob*) req->data;
	dbQueryJob->onProcess();
}

template<class DbMappingClass>
void DbQueryJob<DbMappingClass>::onProcess() {
	DbQueryBinding* binding = dbBinding;
	outputLines.clear();

	//check enabled here so onDone is called
	if(!binding || canceled) {
		done = false;
		return;
	}
	if(onPreProcess() == false) {
		log(LL_Debug, "Canceled DB query in preprocess step\n");
		return;
	}
	done = binding->process(this);

	onPostProcess();
}

template<class DbMappingClass>
void DbQueryJob<DbMappingClass>::onDoneStatic(uv_work_t *req, int status) {
	DbQueryJob* dbQueryJob = (DbQueryJob*) req->data;
	if(status == UV_ECANCELED)
		dbQueryJob->onDone(S_Canceled);
	else if(status == 0 && dbQueryJob->done == true)
		dbQueryJob->onDone(S_Ok);
	else
		dbQueryJob->onDone(S_Error);

	delete dbQueryJob;
}

template<class DbMappingClass>
void *DbQueryJob<DbMappingClass>::createNextLineInstance() {
	outputLines.push_back(std::unique_ptr<OutputType>(new OutputType()));
	return outputLines.back().get();
}

#endif // DBQUERYJOB_IMPL_H
