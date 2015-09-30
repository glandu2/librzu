#ifndef DBQUERYJOB_H
#define DBQUERYJOB_H

#include "Core/Object.h"
#include "uv.h"
#include "Core/EventLoop.h"
#include <type_traits>
#include <memory>
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
	virtual void* createNextLineInstance() = 0;
};

template<class DbMappingClass>
class DbQueryJob : public Object, public IDbQueryJob {
protected:
	template<typename FieldType>
	struct IfValidFieldType : public std::integral_constant<bool, std::is_fundamental<FieldType>::value || std::is_same<FieldType, SQL_TIMESTAMP_STRUCT>::value || std::is_same<FieldType, std::string>::value> {};

public:
	typedef typename DbMappingClass::Input InputType;
	typedef typename DbMappingClass::Output OutputType;
	static const char* SQL_CONFIG_NAME; // defined inside each mapping class

	static bool init(DbConnectionPool* dbConnectionPool); // defined inside each mapping class
	static void deinit();
	DbQueryJob() : done(false), canceled(false) {}

	template<typename FieldType>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addParam(std::vector<DbQueryBinding::ParameterBinding>& params, const char* columnName, FieldType InputType::*member, SQLLEN InputType::*nullIndicator = nullptr) {
		addParam<FieldType>(params, columnName, (size_t)&(((InputType*)0)->*member), nullIndicator);
	}

	template<typename FieldType, int SIZE>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addParam(std::vector<DbQueryBinding::ParameterBinding>& params, const char* columnName, FieldType (InputType::*member)[SIZE], int arrayIndex, SQLLEN InputType::*nullIndicator = nullptr) {
		addParam<FieldType>(params, columnName, (size_t)&(((InputType*)0)->*member[arrayIndex]), nullIndicator);
	}

	template<typename FieldType, int N, int M>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addParam(std::vector<DbQueryBinding::ParameterBinding>& params, const char* columnName, FieldType (InputType::*member)[N][M], int arrayIndexN, int arrayIndexM, SQLLEN InputType::*nullIndicator = nullptr) {
		addParam<FieldType>(params, columnName, (size_t)&(((InputType*)0)->*member[arrayIndexN][arrayIndexM]), nullIndicator);
	}

	template<int SIZE>
	static void addParam(std::vector<DbQueryBinding::ParameterBinding>& params, const char* columnName, char (InputType::*member)[SIZE], SQLLEN InputType::*nullIndicator = nullptr) {
		addParam<char[SIZE]>(params, columnName, (size_t)&(((InputType*)0)->*member), nullIndicator);
	}

	template<typename FieldType>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addColumn(std::vector<DbQueryBinding::ColumnBinding>& columns, const char* columnName, FieldType OutputType::*member, bool OutputType::*nullIndicator = nullptr) {
		addColumn<FieldType>(columns, columnName, (size_t)&(((OutputType*)0)->*member), 0, nullIndicator);
	}

	template<typename FieldType, int SIZE>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addColumn(std::vector<DbQueryBinding::ColumnBinding>& columns, const char* columnName, FieldType (OutputType::*member)[SIZE], int arrayIndex, bool OutputType::*nullIndicator = nullptr) {
		addColumn<FieldType>(columns, columnName, (size_t)&((((OutputType*)0)->*member)[arrayIndex]), 0, nullIndicator);
	}

	template<typename FieldType, int N, int M>
	static typename std::enable_if<IfValidFieldType<FieldType>::value, void>::type
	addColumn(std::vector<DbQueryBinding::ColumnBinding>& columns, const char* columnName, FieldType (OutputType::*member)[N][M], int arrayIndexN, int arrayIndexM, bool OutputType::*nullIndicator = nullptr) {
		addColumn<FieldType>(columns, columnName, (size_t)&((((OutputType*)0)->*member)[arrayIndexN][arrayIndexM]), 0, nullIndicator);
	}

	template<int SIZE>
	static void addColumn(std::vector<DbQueryBinding::ColumnBinding>& columns, const char* columnName, char (OutputType::*member)[SIZE], bool OutputType::*nullIndicator = nullptr) {
		addColumn<char[SIZE]>(columns, columnName, (size_t)&(((OutputType*)0)->*member), SIZE-1, nullIndicator);
	}


	static void executeNoResult(const InputType& input);

	bool execute(const InputType& input);
	void cancel();
	bool isDone() { return done; }
	std::vector<std::unique_ptr<OutputType> >& getResults() { return outputLines; }
	InputType* getInput() { return &input; }

protected:
	static void onProcessStatic(uv_work_t *req);
	void onProcess();

	static void onDoneStatic(uv_work_t *req, int status);

	template<typename FieldType>
	static void addParam(std::vector<DbQueryBinding::ParameterBinding>& params, const char* columnName, size_t memberOffset, SQLLEN InputType::*nullIndicator);

	template<typename FieldType>
	static void addColumn(std::vector<DbQueryBinding::ColumnBinding>& columns, const char* columnName, size_t memberOffset, SQLLEN size, bool OutputType::*nullIndicator);

protected:
	virtual void* createNextLineInstance();

protected:
	~DbQueryJob() {}

private:
	uv_work_t req;
	InputType input;
	std::vector<std::unique_ptr<OutputType> > outputLines;
	bool done;
	volatile bool canceled;

	static DbQueryBinding* dbBinding;
};

template<class DbMappingClass> template<typename FieldType>
void DbQueryJob<DbMappingClass>::addParam(std::vector<DbQueryBinding::ParameterBinding>& params, const char* columnName, size_t memberOffset, SQLLEN InputType::*nullIndicator) {
	typedef typename std::remove_reference<FieldType>::type ValueFieldType;
	char configName[512];
	sprintf(configName, "sql.%s.param.%s", SQL_CONFIG_NAME, columnName);
	params.emplace_back(DbQueryBinding::ParameterBinding(
							ConfigInfo::get()->createValue<cval>(configName, (int)params.size() + 1),
							DbTypeBinding<ValueFieldType>::C_TYPE,
							DbTypeBinding<ValueFieldType>::SQL_TYPE,
							DbTypeBinding<ValueFieldType>::SQL_SIZE,
							DbTypeBinding<ValueFieldType>::SQL_PRECISION,
							IsStdString<ValueFieldType>::value,
							memberOffset,
							nullIndicator ? (size_t)(&(((InputType*)0)->*nullIndicator)) : (size_t)-1));
}

template<class DbMappingClass> template<typename FieldType>
void DbQueryJob<DbMappingClass>::addColumn(std::vector<DbQueryBinding::ColumnBinding>& columns, const char* columnName, size_t memberOffset, SQLLEN size, bool OutputType::*nullIndicator) {
	typedef typename std::remove_reference<FieldType>::type ValueFieldType;
	char configName[512];
	sprintf(configName, "sql.%s.column.%s", SQL_CONFIG_NAME, columnName);
	columns.emplace_back(DbQueryBinding::ColumnBinding(
							 ConfigInfo::get()->createValue<cval>(configName, columnName),
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
void DbQueryJob<DbMappingClass>::executeNoResult(const InputType &input) {
	auto query = new DbQueryJob;
	query->execute(input);
}

template<class DbMappingClass>
bool DbQueryJob<DbMappingClass>::execute(const InputType& input) {
	done = false;
	canceled = false;

	if(dbBinding == nullptr) {
		log(LL_Error, "DB binding was not initialized ! Canceling DB query\n");
		onDone(S_Canceled);
		return false;
	}

	this->input = input;
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
	done = binding->process(this, &input);

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
	outputLines.push_back(std::unique_ptr<OutputType>(new OutputType));
	return outputLines.back().get();
}

#endif // DBQUERYJOB_H
