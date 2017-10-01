#ifndef DBQUERYBINDING_H
#define DBQUERYBINDING_H

#include "Core/Object.h"
#include "DbTypeHelpers.h"
#include <string>
#include <uv.h>  //needed for sqlext on windows
#include <vector>

#include <sqlext.h>

class DbConnectionPool;
class IDbQueryJob;
class DbConnection;
template<typename T> class cval;

class RZU_EXTERN DbQueryBinding : public Object {
	DECLARE_CLASS(DbQueryBinding)
public:
	// Input parameters
	struct ParameterBinding {
		enum Dir { Input, Output };
		typedef void (*PrinterFunction)(std::ostringstream& stream, void* data);

		const char* name;
		PrinterFunction printerFunction;
		SQLSMALLINT way;
		cval<int>* index;
		SQLSMALLINT cType;   // one of SQL_C_* values
		SQLSMALLINT dbType;  // one of SQL_* values
		SQLLEN dbSize;
		SQLSMALLINT dbPrecision;
		bool isStdString;
		size_t bufferOffset;  // use offsetof
		size_t infoPtr;

		ParameterBinding(const char* name,
		                 PrinterFunction printerFunction,
		                 SQLSMALLINT way,
		                 cval<int>& index,
		                 SQLSMALLINT cType,
		                 SQLSMALLINT dbType,
		                 SQLULEN dbSize,
		                 SQLSMALLINT dbPrecision,
		                 bool isStdString,
		                 size_t bufferOffset,
		                 size_t infoPtr)
		    : name(name),
		      printerFunction(printerFunction),
		      way(way),
		      index(&index),
		      cType(cType),
		      dbType(dbType),
		      dbSize(dbSize),
		      dbPrecision(dbPrecision),
		      isStdString(isStdString),
		      bufferOffset(bufferOffset),
		      infoPtr(infoPtr) {}
	};

	// Output data mapping to columns
	struct ColumnBinding {
		cval<std::string>* name;
		SQLSMALLINT cType;
		bool isStdString;
		size_t bufferOffset;  // use offsetof
		SQLLEN bufferSize;
		size_t isNullPtr;

		ColumnBinding(cval<std::string>& name,
		              SQLSMALLINT cType,
		              bool isStdString,
		              size_t bufferOffset,
		              SQLLEN bufferSize,
		              size_t isNullPtr)
		    : name(&name),
		      cType(cType),
		      isStdString(isStdString),
		      bufferOffset(bufferOffset),
		      bufferSize(bufferSize),
		      isNullPtr(isNullPtr) {}
	};

	enum ExecuteMode { EM_NoRow, EM_OneRow, EM_MultiRows };

public:
	DbQueryBinding(DbConnectionPool* dbConnectionPool,
	               cval<bool>& enabled,
	               cval<std::string>& connectionString,
	               cval<std::string>& query,
	               ExecuteMode mode);
	virtual ~DbQueryBinding();

protected:
	template<class T> friend class DbQueryJob;

	bool process(IDbQueryJob* queryJob);
	static void setString(DbConnection* connection,
	                      const ParameterBinding& paramBinding,
	                      SQLLEN* StrLen_or_Ind,
	                      const std::string& str,
	                      std::string& outStr);
	static bool getString(DbConnection* connection, int columnIndex, std::string* outString);
	bool getColumnsMapping(DbConnection* connection, std::vector<const ColumnBinding*>* currentColumnBinding);

	size_t getParameterCount() { return parameterBindings.size(); }
	void addParameter(const ParameterBinding& parameter) { parameterBindings.push_back(parameter); }
	void addColumn(const ColumnBinding& column) { columnBindings.push_back(column); }

	std::string logParameters(void* inputInstance);

private:
	DbConnectionPool* dbConnectionPool;
	cval<bool>& enabled;
	cval<std::string>& connectionString;
	cval<std::string>& query;
	std::vector<ParameterBinding> parameterBindings;
	std::vector<ColumnBinding> columnBindings;
	ExecuteMode mode;
	int errorCount;
	bool columnMappingErrorsShown;
};

#endif  // DBQUERYBINDING_H
