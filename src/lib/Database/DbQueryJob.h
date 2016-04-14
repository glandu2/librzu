#ifndef DBQUERYJOB_H
#define DBQUERYJOB_H

#include "Core/Object.h"
#include "uv.h"
#include <type_traits>
#include <memory>
#include "Config/ConfigInfo.h"
#include "IDbQueryJob.h"
#include "DbQueryBinding.h"
#include "DbBindingLoader.h"

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

#include "DbQueryJob_impl.h"

#endif // DBQUERYJOB_H
