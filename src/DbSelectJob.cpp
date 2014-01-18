#include "DbSelectJob.h"
#include <stdarg.h>
#include <string.h>
#include "EventLoop.h"
#include "Log.h"

static void extractError(Log::Level errorLevel, SQLHANDLE handle, SQLSMALLINT type);

HENV DbSelectJob::henv = nullptr;

bool DbSelectJob::init() {
	SQLRETURN result;
	result = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	if(!SQL_SUCCEEDED(result)) {
		return false;
	}
	result = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
	if(!SQL_SUCCEEDED(result)) {
		Log::get()->log(Log::LL_Error, "DbSelectJob::init", 16, "Can\'t use ODBC 3\n");
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		return false;
	}

	return true;
}

DbSelectJob::DbSelectJob(void* instance, CallbackOnLineDone onLineDone, CallbackOnDone onDone, int argc, const char* query, ...) : onLineDone(onLineDone), onDone(onDone), instance(instance)
{
	va_list args;

	inParams.reserve(argc);
	outParams.reserve(argc);

	va_start(args, query);
	for(int i = 0; i < argc; i++) {
		Parameter param = va_arg(args, Parameter);
		if(param.dir == Parameter::Input)
			inParams.push_back(param);
		else
			outParams.push_back(param);

	}
	va_end(args);

	this->query = new char[strlen(query)];
	strcpy(this->query, query);

	req.data = this;
	uv_queue_work(EventLoop::getLoop(), &req, &onDbProcess, &onDbDone);
}

DbSelectJob::~DbSelectJob() {
	delete this->query;
}

void DbSelectJob::onDbProcess(uv_work_t *req) {
	DbSelectJob* thisInstance = (DbSelectJob*) req->data;
	HDBC hdbc;
	HSTMT hstmt;
	int index;
	std::vector<Parameter>::const_iterator it;

	if(!openConnection(thisInstance->connectionString, &hdbc, &hstmt)) {
		checkError(Log::LL_Debug, &hdbc, &hstmt);
		return;
	}

	thisInstance->trace("Executing query\n");

	for(it = thisInstance->inParams.cbegin(), index = 1; it != thisInstance->inParams.cend(); ++it) {
		const Parameter& param = *it;

		SQLBindParameter(hstmt, index, SQL_PARAM_INPUT, param.cType, param.dbType, param.columnSize, param.decimalDigits, param.data, param.bufferSize, param.info);
		index++;
	}
	SQLExecDirect(hstmt, (SQLCHAR*)thisInstance->query, SQL_NTS);
	thisInstance->trace("Getting data\n");
	while(SQL_SUCCEEDED(SQLFetch(hstmt))) {
		for(it = thisInstance->outParams.cbegin(), index = 1; it != thisInstance->outParams.cend(); ++it) {
			const Parameter& param = *it;

			SQLGetData(hstmt, index, param.cType, param.data, param.bufferSize, param.info);
			index++;
		}

		if(thisInstance->onLineDone)
			thisInstance->onLineDone(thisInstance->instance);
	}

	closeConnection(&hdbc, &hstmt);
}

void DbSelectJob::onDbDone(uv_work_t *req, int status) {
	DbSelectJob* thisInstance = (DbSelectJob*) req->data;

	if(thisInstance->onDone)
		thisInstance->onDone(thisInstance->instance);
}


bool DbSelectJob::openConnection(const std::string& connectionString, void **hdbc, void **hstmt) {
	SQLRETURN result;

	*hdbc = nullptr;
	*hstmt = nullptr;

	//Log::get()->log(Log::LL_Trace, "DB_Account", "Connecting to %s\n", connectionString.c_str());
	result = SQLAllocHandle(SQL_HANDLE_DBC, henv, hdbc);
	if(!SQL_SUCCEEDED(result)) {
		return false;
	}

	result = SQLDriverConnect(*hdbc, nullptr, (UCHAR*)connectionString.c_str(), SQL_NTS, nullptr, 0, nullptr, 0);
	if(!SQL_SUCCEEDED(result)) {
		return false;
	}

	result = SQLAllocHandle(SQL_HANDLE_STMT, *hdbc, hstmt);
	if(!SQL_SUCCEEDED(result)) {
		return false;
	}

	return true;
}

void DbSelectJob::closeConnection(void **hdbc, void **hstmt) {
	if(hstmt && *hstmt) {
		SQLCloseCursor(*hstmt);
		SQLFreeHandle(SQL_HANDLE_STMT, *hstmt);
		*hstmt = nullptr;
	}
	if(hdbc && *hdbc) {
		SQLDisconnect(*hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, *hdbc);
		*hdbc = nullptr;
	}
}

void DbSelectJob::checkError(Log::Level errorLevel, void **hdbc, void **hstmt) {
	if(hstmt && *hstmt)
		extractError(errorLevel, *hstmt, SQL_HANDLE_STMT);
	if(hdbc && *hdbc)
		extractError(errorLevel, *hdbc, SQL_HANDLE_DBC);

	extractError(errorLevel, henv, SQL_HANDLE_ENV);
	closeConnection(hdbc, hstmt);
}

static void extractError(Log::Level errorLevel, SQLHANDLE handle, SQLSMALLINT type) {
	SQLSMALLINT i = 0, len;
	SQLINTEGER native;
	SQLCHAR state[7];
	SQLCHAR text[256];
	SQLRETURN ret;

	do {
		ret = SQLGetDiagRec(type, handle, ++i, state, &native, text, sizeof(text), &len);
		if (SQL_SUCCEEDED(ret))
			Log::get()->log(errorLevel, "ODBCERROR", 9, "%s:%d:%ld:%s\n", state, i, (long)native, text);
	} while(ret == SQL_SUCCESS);
}
