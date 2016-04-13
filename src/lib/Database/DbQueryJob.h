#ifndef DBQUERYJOB_H
#define DBQUERYJOB_H

#include "Core/Object.h"
#include "uv.h"
#include "Core/EventLoop.h"
#include <type_traits>
#include <memory>
#include <assert.h>
#include "Config/ConfigInfo.h"

#include "DbQueryBinding.h"

class IDbQueryJob {
public:
	enum Status {
		S_Ok,
		S_Canceled,
		S_Error
	};

public:
	virtual void cancel() = 0;
	virtual ~IDbQueryJob() {}

public: //events
	//To implement in derived classes
	//In a thread job
	virtual bool onPreProcess() { return true; } //if return false, don't execute the rest (nor the DB query)
	virtual bool onRowDone() { return true; }
	virtual void onPostProcess() {}

	//In main thread
	virtual void onDone(Status status) {}

private:
	friend class DbQueryBinding;
	virtual void* getInputPointer(size_t i) = 0;
	virtual size_t getInputNumber() = 0;
	virtual void* createNextLineInstance() = 0;
};

class RZU_EXTERN DbBindingLoader {
public:
	typedef void (*InitBindingFunction)(DbConnectionPool*);

	static DbBindingLoader* get();
	void addBinding(InitBindingFunction initFunction);
	void initAll(DbConnectionPool* connectionPool);

private:
	DbBindingLoader() {}
	DbBindingLoader(const DbBindingLoader&) {}
	std::vector<InitBindingFunction> dbQueryBindingInits;
};

#define DECLARE_DB_BINDING(classname_, configname_) \
	template<> DbQueryBinding* DbQueryJob<classname_>::dbBinding = (DbBindingLoader::get()->addBinding(&DbQueryJob<classname_>::init), nullptr); \
	template<> const char* DbQueryJob<classname_>::SQL_CONFIG_NAME = configname_


template<class DbMappingClass>
class DbQueryJob : public Object, public IDbQueryJob {
protected:
	template<typename FieldType>
	struct IfValidFieldType : public std::integral_constant<bool, std::is_fundamental<FieldType>::value || std::is_same<FieldType, SQL_TIMESTAMP_STRUCT>::value || std::is_same<FieldType, std::string>::value> {};

public:
	typedef typename DbMappingClass::Input InputType;
	typedef typename DbMappingClass::Output OutputType;
	static const char* SQL_CONFIG_NAME; // defined inside each mapping class

	static void init(DbConnectionPool* dbConnectionPool); // defined inside each mapping class
	static void deinit();
	DbQueryJob() : done(false), canceled(false) {}

	template<typename FieldType>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addParam(const char* columnName, FieldType InputType::*member, SQLLEN InputType::*nullIndicator = nullptr) {
		addParam<FieldType>(columnName, (size_t)&(((InputType*)0)->*member), nullIndicator);
	}

	template<typename FieldType, int SIZE>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addParam(const char* columnName, FieldType (InputType::*member)[SIZE], int arrayIndex, SQLLEN InputType::*nullIndicator = nullptr) {
		addParam<FieldType>(columnName, (size_t)&((((InputType*)0)->*member)[arrayIndex]), nullIndicator);
	}

	template<typename FieldType, int N, int M>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addParam(const char* columnName, FieldType (InputType::*member)[N][M], int arrayIndexN, int arrayIndexM, SQLLEN InputType::*nullIndicator = nullptr) {
		addParam<FieldType>(columnName, (size_t)&((((InputType*)0)->*member)[arrayIndexN][arrayIndexM]), nullIndicator);
	}

	template<int SIZE>
	static void addParam(const char* columnName, char (InputType::*member)[SIZE], SQLLEN InputType::*nullIndicator = nullptr) {
		addParam<char[SIZE]>(columnName, (size_t)&(((InputType*)0)->*member), nullIndicator);
	}

	template<typename FieldType>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addOutputParam(const char* columnName, FieldType InputType::*member, SQLLEN InputType::*nullIndicator = nullptr) {
		addOutputParam<FieldType>(columnName, (size_t)&(((InputType*)0)->*member), nullIndicator);
	}

	template<typename FieldType, int SIZE>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addOutputParam(const char* columnName, FieldType (InputType::*member)[SIZE], int arrayIndex, SQLLEN InputType::*nullIndicator = nullptr) {
		addOutputParam<FieldType>(columnName, (size_t)&(((InputType*)0)->*member[arrayIndex]), nullIndicator);
	}

	template<typename FieldType, int N, int M>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addOutputParam(const char* columnName, FieldType (InputType::*member)[N][M], int arrayIndexN, int arrayIndexM, SQLLEN InputType::*nullIndicator = nullptr) {
		addOutputParam<FieldType>(columnName, (size_t)&(((InputType*)0)->*member[arrayIndexN][arrayIndexM]), nullIndicator);
	}

	template<int SIZE>
	static void addOutputParam(const char* columnName, char (InputType::*member)[SIZE], SQLLEN InputType::*nullIndicator = nullptr) {
		addOutputParam<char[SIZE]>(columnName, (size_t)&(((InputType*)0)->*member), nullIndicator);
	}

	template<typename FieldType>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addColumn(const char* columnName, FieldType OutputType::*member, bool OutputType::*nullIndicator = nullptr) {
		addColumn<FieldType>(columnName, (size_t)&(((OutputType*)0)->*member), 0, nullIndicator);
	}

	template<typename FieldType, int SIZE>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addColumn(const char* columnName, FieldType (OutputType::*member)[SIZE], int arrayIndex, bool OutputType::*nullIndicator = nullptr) {
		addColumn<FieldType>(columnName, (size_t)&((((OutputType*)0)->*member)[arrayIndex]), 0, nullIndicator);
	}

	template<typename FieldType, int N, int M>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addColumn(const char* columnName, FieldType (OutputType::*member)[N][M], int arrayIndexN, int arrayIndexM, bool OutputType::*nullIndicator = nullptr) {
		addColumn<FieldType>(columnName, (size_t)&((((OutputType*)0)->*member)[arrayIndexN][arrayIndexM]), 0, nullIndicator);
	}

	template<int SIZE>
	static void addColumn(const char* columnName, char (OutputType::*member)[SIZE], bool OutputType::*nullIndicator = nullptr) {
		addColumn<char[SIZE]>(columnName, (size_t)&(((OutputType*)0)->*member), SIZE-1, nullIndicator);
	}

	static void createBinding(DbConnectionPool* dbConnectionPool,
										 cval<std::string>& connectionString,
										 const char* query,
										 DbQueryBinding::ExecuteMode executeMode);


	static void executeNoResult(const InputType& input);
	static void executeNoResult(const std::vector<InputType>& input);

	bool execute(const InputType *inputs, size_t number);
	void cancel();
	bool isDone() { return done; }
	std::vector<std::unique_ptr<OutputType> >& getResults() { return outputLines; }
	InputType* getInput(size_t i = 0) { return &input[i]; }

	virtual void* getInputPointer(size_t i) { return &input[i]; }
	virtual size_t getInputNumber() { return input.size(); }

protected:
	static void onProcessStatic(uv_work_t *req);
	void onProcess();

	static void onDoneStatic(uv_work_t *req, int status);

	template<typename FieldType>
	static void addParam(const char* columnName, size_t memberOffset, SQLLEN InputType::*nullIndicator);

	template<typename FieldType>
	static void addOutputParam(const char* columnName, size_t memberOffset, SQLLEN InputType::*nullIndicator);

	template<typename FieldType>
	static void addColumn(const char* columnName, size_t memberOffset, SQLLEN size, bool OutputType::*nullIndicator);

protected:
	virtual void* createNextLineInstance();

protected:
	~DbQueryJob() {}

private:
	uv_work_t req;
	std::vector<InputType> input;
	std::vector<std::unique_ptr<OutputType> > outputLines;
	bool done;
	volatile bool canceled;

	static DbQueryBinding* dbBinding;
};

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

#endif // DBQUERYJOB_H
