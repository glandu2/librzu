#ifndef DBQUERYJOB_H
#define DBQUERYJOB_H

#include "Object.h"
#include "uv.h"
#include <sqlext.h>
#include <vector>
#include <string>
#include "EventLoop.h"

#define OFFSETOF(C, f, type) (size_t((type*)&((C*)1)->f) - 1)
#define TYPEOFFIELD(C, f) decltype(((C*)1)->f)

class DbConnectionPool;
template<typename T> class cval;

template<typename T> struct DbTypeBinding {};

template<> struct DbTypeBinding<bool> { enum { C_TYPE = SQL_C_BIT, SQL_TYPE = SQL_BIT, SQL_SIZE = 1, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<char> { enum { C_TYPE = SQL_C_TINYINT, SQL_TYPE = SQL_TINYINT, SQL_SIZE = 3, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<short> { enum { C_TYPE = SQL_C_SHORT, SQL_TYPE = SQL_SMALLINT, SQL_SIZE = 5, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<int> { enum { C_TYPE = SQL_C_LONG, SQL_TYPE = SQL_INTEGER, SQL_SIZE = 10, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<long long> { enum { C_TYPE = SQL_C_SBIGINT, SQL_TYPE = SQL_BIGINT, SQL_SIZE = 19, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<float> { enum { C_TYPE = SQL_C_FLOAT, SQL_TYPE = SQL_REAL, SQL_SIZE = 7, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<double> { enum { C_TYPE = SQL_C_DOUBLE, SQL_TYPE = SQL_DOUBLE, SQL_SIZE = 15, SQL_PRECISION = 0 }; };

template<> struct DbTypeBinding<unsigned char> : DbTypeBinding<char> {};
template<> struct DbTypeBinding<unsigned short> : DbTypeBinding<short> {};
template<> struct DbTypeBinding<unsigned int> : DbTypeBinding<int> {};
template<> struct DbTypeBinding<unsigned long long> : DbTypeBinding<long long> {};

template<int ARRAY_SIZE> struct DbTypeBinding<char[ARRAY_SIZE]> { enum { C_TYPE = SQL_C_CHAR, SQL_TYPE = SQL_VARCHAR, SQL_SIZE = ARRAY_SIZE, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<std::string> { enum { C_TYPE = SQL_C_CHAR, SQL_TYPE = SQL_VARCHAR, SQL_SIZE = -1, SQL_PRECISION = 0 }; };

template<int ARRAY_SIZE> struct DbTypeBinding<unsigned char[ARRAY_SIZE]> : DbTypeBinding<char(&)[ARRAY_SIZE]> { };

template<typename T> struct IsStdString { enum { value = false }; };
template<> struct IsStdString<std::string> { enum { value = true }; };


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
};

class RZU_EXTERN DbQueryBinding : public Object
{
	DECLARE_CLASS(DbQueryBinding)
public:
	//Input parameters
	struct ParameterBinding {
		enum Dir {
			Input,
			Output
		};

		cval<int>& index;
		SQLSMALLINT cType; //one of SQL_C_* values
		SQLSMALLINT dbType; //one of SQL_* values
		SQLLEN dbSize;
		SQLSMALLINT dbPrecision;
		bool isStdString;
		size_t bufferOffset; //use offsetof
		SQLLEN bufferSize;
		size_t infoPtr;

		ParameterBinding(cval<int>& index, SQLSMALLINT cType, SQLSMALLINT dbType, SQLULEN dbSize, SQLSMALLINT dbPrecision, bool isStdString, size_t bufferOffset, SQLLEN bufferSize, size_t infoPtr = 0)
			: index(index), cType(cType), dbType(dbType), dbSize(dbSize), dbPrecision(dbPrecision), isStdString(isStdString), bufferOffset(bufferOffset), bufferSize(bufferSize), infoPtr(infoPtr) {}

		#define DECLARE_PARAMETER(C, field, size, index) \
			DbQueryBinding::ParameterBinding(index, DbTypeBinding<TYPEOFFIELD(C, field)>::C_TYPE, DbTypeBinding<TYPEOFFIELD(C, field)>::SQL_TYPE, DbTypeBinding<TYPEOFFIELD(C, field)>::SQL_SIZE, DbTypeBinding<TYPEOFFIELD(C, field)>::SQL_PRECISION, IsStdString<TYPEOFFIELD(C, field)>::value, OFFSETOF(C, field, const void), size)
		#define DECLARE_PARAMETER_WITH_INFO(C, field, size, index, info) \
			DbQueryBinding::ParameterBinding(index, DbTypeBinding<TYPEOFFIELD(C, field)>::C_TYPE, DbTypeBinding<TYPEOFFIELD(C, field)>::SQL_TYPE, DbTypeBinding<TYPEOFFIELD(C, field)>::SQL_SIZE, DbTypeBinding<TYPEOFFIELD(C, field)>::SQL_PRECISION, IsStdString<TYPEOFFIELD(C, field)>::value, OFFSETOF(C, field, const void), size, OFFSETOF(C, info, SQLLEN))

	};

	//Output data mapping to columns
	struct ColumnBinding {
		cval<std::string>& name;
		SQLSMALLINT cType;
		bool isStdString;
		size_t bufferOffset; //use offsetof
		SQLLEN bufferSize;
		size_t isNullPtr;

		ColumnBinding(cval<std::string>& name, SQLSMALLINT cType, bool isStdString, size_t bufferOffset, SQLLEN bufferSize, size_t isNullPtr = 0)
			: name(name), cType(cType), isStdString(isStdString), bufferOffset(bufferOffset), bufferSize(bufferSize), isNullPtr(isNullPtr) {}


		#define DECLARE_COLUMN(C, field, size, name) \
			DbQueryBinding::ColumnBinding(name, DbTypeBinding<TYPEOFFIELD(C, field)>::C_TYPE, IsStdString<TYPEOFFIELD(C, field)>::value, OFFSETOF(C, field, void), size)
		#define DECLARE_COLUMN_WITH_INFO(C, field, size, name, isNullPtr) \
			DbQueryBinding::ColumnBinding(name, DbTypeBinding<TYPEOFFIELD(C, field)>::C_TYPE, IsStdString<TYPEOFFIELD(C, field)>::value, OFFSETOF(C, field, void), size, OFFSETOF(C, isNullPtr, bool))

	};

	enum ExecuteMode {
		EM_NoRow,
		EM_OneRow,
		EM_MultiRows
	};

public:
	DbQueryBinding(DbConnectionPool* dbConnectionPool, cval<bool>& enabled, cval<std::string>& connectionString, cval<std::string>& query, const std::vector<ParameterBinding>& parameters, const std::vector<ColumnBinding>& columns);
	virtual ~DbQueryBinding();


protected:
	template<class T> friend class DbQueryJob;
	bool process(IDbQueryJob* queryJob, void* instance, ExecuteMode mode);

private:
	DbConnectionPool* dbConnectionPool;
	cval<bool>& enabled;
	cval<std::string>& connectionString;
	cval<std::string>& query;
	std::vector<ParameterBinding> parameterBindings;
	std::vector<ColumnBinding> columnBindings;
	int errorCount;
};

template<class T>
class DbQueryJob : public Object, public IDbQueryJob {
public:
	typedef DbQueryBinding::ExecuteMode ExecuteMode;
public:
	DbQueryJob() : instance(nullptr), done(false), canceled(false) {}

	bool execute(ExecuteMode mode) {
		done = false;
		canceled = false;
		this->mode = mode;

		if(dbBinding == nullptr) {
			error("DB binding was not initialized ! Canceling DB query\n");
			onDone(S_Canceled);
			return false;
		}

		instance = static_cast<T*>(this);
		req.data = this;
		uv_queue_work(EventLoop::getLoop(), &req, &onProcessStatic, &onDoneStatic);

		return true;
	}

	void cancel() {
		canceled = true;
		uv_cancel((uv_req_t*)&req);
	}

	bool isDone() { return done; }

protected:
	static void onProcessStatic(uv_work_t *req) {
		DbQueryJob* dbQueryJob = (DbQueryJob*) req->data;
		dbQueryJob->onProcess();
	}

	void onProcess() {
		DbQueryBinding* binding = dbBinding;

		//check enabled here so onDone is called
		if(!binding || canceled) {
			done = false;
			return;
		}
		if(onPreProcess() == false) {
			debug("Canceled DB query in preprocess step\n");
			return;
		}
		done = binding->process(this, instance, mode);

		onPostProcess();
	}

	static void onDoneStatic(uv_work_t *req, int status) {
		DbQueryJob* dbQueryJob = (DbQueryJob*) req->data;
		if(status == UV_ECANCELED)
			dbQueryJob->onDone(S_Canceled);
		else if(status == 0 && dbQueryJob->done == true)
			dbQueryJob->onDone(S_Ok);
		else
			dbQueryJob->onDone(S_Error);

		delete dbQueryJob;
	}

protected:
	~DbQueryJob() {}

	static DbQueryBinding* dbBinding;

private:
	uv_work_t req;
	void* instance;
	ExecuteMode mode;
	bool done;
	volatile bool canceled;
};

#endif // DBQUERYJOB_H
