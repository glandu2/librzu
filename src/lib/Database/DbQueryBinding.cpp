#include "DbQueryBinding.h"
#include <string.h>
#include "DbConnectionPool.h"
#include "DbConnection.h"
#include "DbQueryJob.h"
#include "Config/ConfigParamVal.h"
#include "Core/CharsetConverter.h"
#include "Config/GlobalCoreConfig.h"

DbQueryBinding::DbQueryBinding(DbConnectionPool* dbConnectionPool,
					   cval<bool>& enabled,
					   cval<std::string>& connectionString,
					   cval<std::string>& query,
					   ExecuteMode mode)
	: dbConnectionPool(dbConnectionPool),
	  enabled(enabled),
	  connectionString(connectionString),
	  query(query),
	  mode(mode),
	  errorCount(0)
{
}

DbQueryBinding::~DbQueryBinding() {
}

bool DbQueryBinding::process(IDbQueryJob* queryJob, void* inputInstance) {
	struct UnicodeString {
		std::string str;
		SQLLEN strLen;
	};

	DbConnection* connection;
	bool columnCountOk;
	std::vector<UnicodeString> unicodeStrings;

	if(enabled.get() == false) {
		errorCount = 0;
		return true;
	}

	std::string queryStr = query.get();

	connection = dbConnectionPool->getConnection(connectionString.get().c_str(), queryStr);
	if(!connection) {
		log(LL_Warning, "Could not retrieve a DB connection from pool\n");
		return false;
	}


	log(LL_Trace, "Executing: %s\n", queryStr.c_str());

	for(size_t i = 0; i < parameterBindings.size(); i++) {
		const ParameterBinding& paramBinding = parameterBindings.at(i);

		if(paramBinding.index > 0) {
			SQLLEN* StrLen_or_Ind;
			if(paramBinding.infoPtr != (size_t)-1)
				StrLen_or_Ind = (SQLLEN*)((char*)inputInstance +  paramBinding.infoPtr);
			else
				StrLen_or_Ind = nullptr;

			if(paramBinding.isStdString) {
				std::string* str = (std::string*) ((char*)inputInstance + paramBinding.bufferOffset);
				unicodeStrings.push_back(UnicodeString());
				UnicodeString& unicodeString = unicodeStrings.back();

				if(!StrLen_or_Ind)
					StrLen_or_Ind = &unicodeString.strLen;

				setString(connection, paramBinding, StrLen_or_Ind, *str, unicodeString.str);
				unicodeString.strLen = unicodeString.str.size();
			} else {
				connection->bindParameter(paramBinding.index, SQL_PARAM_INPUT,
										  paramBinding.cType,
										  paramBinding.dbType, paramBinding.dbSize, paramBinding.dbPrecision,
										  (char*)inputInstance + paramBinding.bufferOffset, 0,
										  StrLen_or_Ind);
			}
			//TODO: print content of params buffer
		}
	}

	if(!connection->execute(queryStr.c_str())) {
		connection->releaseWithError();
		log(LL_Warning, "DB query failed: %s\n", queryStr.c_str());
		errorCount++;
		if(errorCount > 10) {
			enabled.setBool(false);
			log(LL_Error, "Disabled query: %s, too many errors\n", queryStr.c_str());

			errorCount = 0;
		}
		return false;
	}

	if(mode != EM_NoRow) {
		bool firstRowFetched = false;
		while(connection->fetch() && (firstRowFetched == false || mode == EM_MultiRows)) {
			firstRowFetched = true;

			log(LL_Trace, "Fetching data\n");
			const int columnCount = connection->getColumnNum(&columnCountOk);

			if(!columnCountOk) {
				connection->releaseWithError();
				return false;
			}

			void* outputInstance = queryJob->createNextLineInstance();

			char columnName[32];
			for(int col = 0; col < columnCount; col++) {
				const int columnIndex = col + 1;
				SQLLEN StrLen_Or_Ind;

				connection->getColumnName(columnIndex, columnName, sizeof(columnName));

				//TODO: cache column to get indices
				for(size_t i = 0; i < columnBindings.size(); i++) {
					const ColumnBinding& columnBinding = columnBindings.at(i);

					if(!strcmp(columnName, columnBinding.name.get().c_str())) {
						if(columnBinding.isStdString)
						{
							std::string* str = (std::string*) ((char*)outputInstance + columnBinding.bufferOffset);
							*str = getString(connection, columnIndex);
						} else {
							connection->getData(columnIndex, columnBinding.cType,
												(char*)outputInstance + columnBinding.bufferOffset, columnBinding.bufferSize,
												&StrLen_Or_Ind);
						}

						if(columnBinding.isNullPtr != (size_t)-1)
							*(bool*)((char*)outputInstance + columnBinding.isNullPtr) = StrLen_Or_Ind == SQL_NULL_DATA;

						break;
					}
				}
			}

			if(queryJob->onRowDone() == false)
				break;
		}
	}

	connection->release();

	return true;
}

void DbQueryBinding::setString(DbConnection* connection, const ParameterBinding& paramBinding, SQLLEN* StrLen_or_Ind, const std::string& str, std::string &outStr) {
	CharsetConverter localToUtf16(CharsetConverter::getEncoding().c_str(), "UTF-16LE");
	localToUtf16.convert(str, outStr, 2);

	// If the string is empty, put a size of 1 else SQL server complains about invalid precision
	connection->bindParameter(paramBinding.index, SQL_PARAM_INPUT,
							  SQL_C_WCHAR,
							  SQL_WVARCHAR, str.size() > 0 ? str.size() : 1, paramBinding.dbPrecision,
							  (SQLPOINTER)outStr.data(), 0,
							  StrLen_or_Ind);
}

std::string DbQueryBinding::getString(DbConnection* connection, int columnIndex) {
	std::string unicodeBuffer;
	SQLLEN bytesRead = 0;
	SQLLEN dataSize;
	SQLLEN isDataNull;
	int dummy;
	SQLRETURN ret;

	connection->getData(columnIndex, SQL_C_BINARY, &dummy, 0, &dataSize, true);
	unicodeBuffer.resize(dataSize*2 + 4);

	while(connection->getData(columnIndex, SQL_C_WCHAR, &unicodeBuffer[bytesRead], unicodeBuffer.size() - bytesRead, &isDataNull, false, &ret) && ret == SQL_SUCCESS_WITH_INFO) {
		if(isDataNull == SQL_NULL_DATA)
			break;

		bytesRead = unicodeBuffer.size()-2; //dont keep null terminator
		unicodeBuffer.resize(unicodeBuffer.size()*2);
	}
	if(ret == SQL_SUCCESS) {
		bytesRead += isDataNull;
		unicodeBuffer.resize(bytesRead);
	}

	if(isDataNull != SQL_NULL_DATA && bytesRead != 0) {
		CharsetConverter utf16ToLocal("UTF-16LE", CharsetConverter::getEncoding().c_str());
		std::string result;
		utf16ToLocal.convert(unicodeBuffer, result, 0.5);
		return result;
	}

	return std::string();
}
