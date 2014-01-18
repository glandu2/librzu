#ifndef DBSELECTJOB_H
#define DBSELECTJOB_H

#include "Object.h"
#include "uv.h"
#include <sqlext.h>
#include <vector>
#include <string>
#include "Log.h"

class DbSelectJob : public Object
{
public:
	struct Parameter {
		enum Dir {
			Input,
			Output
		} dir;

		SQLSMALLINT cType; //one of SQL_C_* values
		SQLSMALLINT dbType; //one of SQL_* values
		SQLULEN columnSize;
		SQLSMALLINT decimalDigits;
		SQLPOINTER data;
		SQLLEN bufferSize;
		SQLLEN *info;

		Parameter(Dir dir, SQLSMALLINT cType, SQLSMALLINT dbType, SQLULEN columnSize, SQLSMALLINT decimalDigits, SQLPOINTER data, SQLLEN bufferSize, SQLLEN* info = nullptr) : dir(dir), cType(cType), dbType(dbType), columnSize(columnSize), decimalDigits(decimalDigits), data(data), bufferSize(bufferSize), info(info) {}
	};

	typedef void (*CallbackOnLineDone)(void* instance);
	typedef void (*CallbackOnDone)(void* instance);

public:
	//after query, type must be Parameter values
	DbSelectJob(void* instance, CallbackOnLineDone onLineDone, CallbackOnDone onDone, int argc, const char* query, ...);
	virtual ~DbSelectJob();

	static bool init();

private:
	static void onDbProcess(uv_work_t *req);
	static void onDbDone(uv_work_t *req, int status);
	static bool openConnection(const std::string &connectionString, void **hdbc, void **hstmt);
	static void closeConnection(void **hdbc, void **hstmt);
	static void checkError(Log::Level errorLevel, void **hdbc, void **hstmt);

	//one sql env for all connection
	static HENV henv;
	uv_work_t req;
	std::string connectionString;
	char* query;
	std::vector<Parameter> inParams;
	std::vector<Parameter> outParams;

	CallbackOnLineDone onLineDone;
	CallbackOnDone onDone;
	void* instance;
};

#endif // DBSELECTJOB_H
