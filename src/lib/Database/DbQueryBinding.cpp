#include "DbQueryBinding.h"
#include "Config/ConfigParamVal.h"
#include "Core/CharsetConverter.h"
#include "Core/PrintfFormats.h"
#include "DbConnection.h"
#include "DbConnectionPool.h"
#include "DbString.h"
#include "IDbQueryJob.h"
#include <algorithm>
#include <string.h>

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
      errorCount(0),
      columnMappingErrorsShown(false) {}

DbQueryBinding::~DbQueryBinding() {}

bool DbQueryBinding::getColumnsMapping(DbConnection* connection,
                                       std::vector<const ColumnBinding*>* currentColumnBinding) {
	bool columnCountOk;
	bool getDataErrorOccured = false;
	bool showErrors = columnMappingErrorsShown == false;
	columnMappingErrorsShown = true;

	const int columnCount = connection->getColumnNum(&columnCountOk);
	if(!columnCountOk) {
		connection->releaseWithError();
		log(LL_Error, "Coulnd't retrieve column count\n");
		return false;
	}

	std::vector<std::string> columnNames;
	columnNames.reserve(columnCount);

	for(int col = 0; col < columnCount; col++) {
		char columnName[128];
		connection->getColumnName(col + 1, columnName, sizeof(columnName));
		columnNames.push_back(std::string(columnName));
	}

	currentColumnBinding->resize(columnNames.size());
	for(size_t i = 0; i < columnBindings.size(); i++) {
		const ColumnBinding& columnBinding = columnBindings[i];
		std::string columnBindingName = columnBinding.name->get();
		bool found = false;

		for(size_t col = 0; col < columnNames.size(); col++) {
			if(columnNames[col] == columnBindingName) {
				found = true;
				(*currentColumnBinding)[col] = &columnBinding;
				break;
			}
		}

		if(!found && showErrors) {
			log(LL_Warning, "Column %s not found in query result set\n", columnBindingName.c_str());
			getDataErrorOccured = true;
		}
	}

	return !getDataErrorOccured;
}

std::string DbQueryBinding::logParameters(void* inputInstance) {
	std::ostringstream logData;

	for(size_t i = 0; i < parameterBindings.size(); i++) {
		const ParameterBinding& paramBinding = parameterBindings.at(i);

		if(*paramBinding.index > 0) {
			logData << " - " << paramBinding.name << ": ";

			if(paramBinding.isStdString == DVT_StdString) {
				std::string* str = (std::string*) ((char*) inputInstance + paramBinding.bufferOffset);
				logData << "\"" << *str << "\"\n";
			} else {
				logData << "\"";
				paramBinding.printerFunction(logData, (char*) inputInstance + paramBinding.bufferOffset);
				logData << "\"\n";
			}
		}
	}

	return logData.str();
}

bool DbQueryBinding::process(IDbQueryJob* queryJob) {
	struct UnicodeString {
		std::string str;
		SQLLEN strLen;
	};

	DbConnection* connection;
	std::vector<UnicodeString> unicodeStrings;

	if(enabled.get() == false) {
		errorCount = 0;
		return false;
	}

	// If NoRow and no std::string row, use processBatch
	if(mode == EM_NoRow &&
	   std::count_if(parameterBindings.begin(), parameterBindings.end(), [](const ParameterBinding& param) {
		   return param.isStdString == DVT_StdString;
	   }) == 0) {
		return processBatch(queryJob);
	}

	std::string queryStr = query.get();

	connection = dbConnectionPool->getConnection(connectionString.get().c_str(), queryStr);
	if(!connection) {
		log(LL_Warning, "Could not retrieve a DB connection from pool\n");
		return false;
	}

	connection->setAutoCommit(false);

	bool getDataErrorOccured = false;  // if true, show query after all getData

	size_t inputNumber = queryJob->getInputNumber();
	for(size_t inputIndex = 0; inputIndex < inputNumber && !getDataErrorOccured; inputIndex++) {
		void* inputInstance = queryJob->getInputPointer(inputIndex);

		log(LL_Trace, "Executing: %s\n", queryStr.c_str());

		for(size_t i = 0; i < parameterBindings.size(); i++) {
			const ParameterBinding& paramBinding = parameterBindings.at(i);

			if(*paramBinding.index > 0) {
				SQLLEN* StrLen_or_Ind;
				if(paramBinding.infoPtr != (size_t) -1)
					StrLen_or_Ind = (SQLLEN*) ((char*) inputInstance + paramBinding.infoPtr);
				else
					StrLen_or_Ind = nullptr;

				if(paramBinding.isStdString) {
					std::string* str = (std::string*) ((char*) inputInstance + paramBinding.bufferOffset);
					unicodeStrings.push_back(UnicodeString());
					UnicodeString& unicodeString = unicodeStrings.back();

					if(!StrLen_or_Ind)
						StrLen_or_Ind = &unicodeString.strLen;

					setString(connection, paramBinding, StrLen_or_Ind, *str, unicodeString.str);
					unicodeString.strLen = unicodeString.str.size();
				} else {
					connection->bindParameter(*paramBinding.index,
					                          paramBinding.way,
					                          paramBinding.cType,
					                          paramBinding.dbType,
					                          paramBinding.dbSize,
					                          paramBinding.dbPrecision,
					                          (char*) inputInstance + paramBinding.bufferOffset,
					                          0,
					                          StrLen_or_Ind);
				}
			}
		}

		if(!connection->execute(queryStr.c_str())) {
			connection->releaseWithError();
			if(parameterBindings.empty())
				log(LL_Error, "DB query failed: %s\n", queryStr.c_str());
			else
				log(LL_Error,
				    "DB query failed: %s\nParameters:\n%s",
				    queryStr.c_str(),
				    logParameters(inputInstance).c_str());
			errorCount++;
			if(errorCount > 100) {
				enabled.setBool(false);
				log(LL_Error, "Disabled query: %s, too many errors\n", queryStr.c_str());

				errorCount = 0;
			}
			return false;
		}

		if(mode != EM_NoRow) {
			log(LL_Trace, "Fetching data\n");
			std::vector<const ColumnBinding*> currentColumnBinding;
			getDataErrorOccured = !getColumnsMapping(connection, &currentColumnBinding);

			size_t rowFetched = 0;
			while(connection->fetch() && (rowFetched == 0 || mode == EM_MultiRows)) {
				rowFetched++;

				if(!currentColumnBinding.empty()) {
					void* outputInstance = queryJob->createNextLineInstance();

					for(int col = 0; col < (int) currentColumnBinding.size(); col++) {
						const int columnIndex = col + 1;

						const ColumnBinding* columnBinding = currentColumnBinding[col];
						if(columnBinding) {
							bool getDataSucceded;
							if(columnBinding->isStdString) {
								std::string* str =
								    (std::string*) ((char*) outputInstance + columnBinding->bufferOffset);
								getDataSucceded = getString(connection, columnIndex, str);
							} else {
								SQLLEN StrLen_Or_Ind = 0;

								getDataSucceded =
								    connection->getData(columnIndex,
								                        columnBinding->cType,
								                        (char*) outputInstance + columnBinding->bufferOffset,
								                        columnBinding->bufferSize,
								                        &StrLen_Or_Ind);

								if(columnBinding->isNullPtr != (size_t) -1)
									*(bool*) ((char*) outputInstance + columnBinding->isNullPtr) =
									    StrLen_Or_Ind == SQL_NULL_DATA;
							}

							if(!getDataSucceded) {
								log(LL_Error,
								    "Failed to retrieve data for column %s(%d) for line %d\n",
								    columnBinding->name->get().c_str(),
								    columnIndex,
								    (int) rowFetched);
								getDataErrorOccured = true;
							}
						}
					}

					if(queryJob->onRowDone() == false)
						break;
				}
			}
		}

		connection->closeCursor();
	}

	connection->endTransaction(getDataErrorOccured == false);
	connection->release();

	if(getDataErrorOccured) {
		log(LL_Error, "Errors occured while retrieving data for query %s\n", queryStr.c_str());
	}

	return true;
}

bool DbQueryBinding::processBatch(IDbQueryJob* queryJob) {
	DbConnection* connection;
	std::vector<SQLUSMALLINT> paramStatus;
	SQLULEN paramsProcessed;

	std::string queryStr = query.get();

	connection = dbConnectionPool->getConnection(connectionString.get().c_str(), queryStr);
	if(!connection) {
		log(LL_Warning, "Could not retrieve a DB connection from pool\n");
		return false;
	}

	connection->setAutoCommit(false);

	size_t inputNumber = queryJob->getInputNumber();
	paramStatus.resize(inputNumber, SQL_PARAM_UNUSED);
	connection->setAttribute(SQL_ATTR_PARAM_BIND_TYPE, (void*) queryJob->getInputSize(), 0);
	connection->setAttribute(SQL_ATTR_PARAMSET_SIZE, (void*) inputNumber, 0);
	connection->setAttribute(SQL_ATTR_PARAM_STATUS_PTR, (void*) paramStatus.data(), 0);
	connection->setAttribute(SQL_ATTR_PARAMS_PROCESSED_PTR, (void*) &paramsProcessed, 0);

	// Convert all DbString to unicode
	{
		std::string unicodeStr;
		for(size_t inputIndex = 0; inputIndex < inputNumber; inputIndex++) {
			void* inputInstance = queryJob->getInputPointer(inputIndex);

			for(const ParameterBinding& paramBinding : parameterBindings) {
				if(paramBinding.isStdString == DVT_DbString) {
					DbStringBase* str = (DbStringBase*) ((char*) inputInstance + paramBinding.bufferOffset);

					unicodeStr.clear();
					CharsetConverter localToUtf16(CharsetConverter::getEncoding().c_str(), "UTF-16LE");
					localToUtf16.convert(str->to_string(), unicodeStr, 2);
					str->assign(unicodeStr);
				}
			}
		}
	}

	// Bind first row
	void* inputInstance = queryJob->getInputPointer(0);
	for(const ParameterBinding& paramBinding : parameterBindings) {
		if(*paramBinding.index > 0) {
			SQLLEN* StrLen_or_Ind = nullptr;

			if(paramBinding.infoPtr != (size_t) -1)
				StrLen_or_Ind = (SQLLEN*) ((char*) inputInstance + paramBinding.infoPtr);

			if(paramBinding.isStdString == DVT_DbString) {
				DbStringBase* str = (DbStringBase*) ((char*) inputInstance + paramBinding.bufferOffset);

				if(!StrLen_or_Ind)
					StrLen_or_Ind = str->sql_size_ptr();

				SQLLEN strLen = str->size();

				// If the string is empty, put a size of 1 else SQL server complains about invalid precision
				connection->bindParameter(*paramBinding.index,
				                          paramBinding.way,
				                          paramBinding.cType,
				                          paramBinding.dbType,
				                          strLen > 0 ? strLen : 1,
				                          paramBinding.dbPrecision,
				                          str->data(),
				                          str->max_size(),
				                          StrLen_or_Ind);
			} else {
				connection->bindParameter(*paramBinding.index,
				                          paramBinding.way,
				                          paramBinding.cType,
				                          paramBinding.dbType,
				                          paramBinding.dbSize,
				                          paramBinding.dbPrecision,
				                          (char*) inputInstance + paramBinding.bufferOffset,
				                          0,
				                          StrLen_or_Ind);
			}
		}
	}

	log(LL_Trace, "Executing: %s\n", queryStr.c_str());

	if(!connection->execute(queryStr.c_str())) {
		if(parameterBindings.empty()) {
			log(LL_Error, "DB query failed: %s\n", queryStr.c_str());
		} else {
			for(size_t i = 0; i < paramStatus.size(); i++) {
				SQLRETURN result;
				const char* statusStr;

				switch(paramStatus[i]) {
					case SQL_PARAM_SUCCESS:
						result = SQL_SUCCESS;
						statusStr = "success";
						break;
					case SQL_PARAM_SUCCESS_WITH_INFO:
						result = SQL_SUCCESS_WITH_INFO;
						statusStr = "success with info";
						break;

					case SQL_PARAM_ERROR:
						result = SQL_ERROR;
						statusStr = "error";
						break;

					case SQL_PARAM_UNUSED:
						result = SQL_SUCCESS;
						statusStr = "result unused";
						break;

					case SQL_PARAM_DIAG_UNAVAILABLE:
						result = SQL_ERROR;
						statusStr = "status unavailable";
						break;

					default:
						result = SQL_ERROR;
						statusStr = "status unknown";
						break;
				}

				if(!connection->checkResult(result, "SQLExecute")) {
					log(LL_Error,
					    "DB query %s: %s\nParameters:\n%s",
					    queryStr.c_str(),
					    statusStr,
					    logParameters(queryJob->getInputPointer(i)).c_str());
				}
			}
		}

		errorCount++;
		if(errorCount > 100) {
			enabled.setBool(false);
			log(LL_Error, "Disabled query: %s, too many errors\n", queryStr.c_str());

			errorCount = 0;
		}
		connection->releaseWithError();
		return false;
	} else {
		for(size_t i = 0; i < paramStatus.size(); i++) {
			SQLRETURN result;
			const char* statusStr;

			switch(paramStatus[i]) {
				case SQL_PARAM_SUCCESS:
					result = SQL_SUCCESS;
					statusStr = "success";
					break;
				case SQL_PARAM_SUCCESS_WITH_INFO:
					result = SQL_SUCCESS_WITH_INFO;
					statusStr = "success with info";
					break;

				case SQL_PARAM_ERROR:
					result = SQL_ERROR;
					statusStr = "error";
					break;

				case SQL_PARAM_UNUSED:
					result = SQL_ERROR;
					statusStr = "result unused";
					break;

				case SQL_PARAM_DIAG_UNAVAILABLE:
					result = SQL_ERROR;
					statusStr = "status unavailable";
					break;

				default:
					result = SQL_ERROR;
					statusStr = "status unknown";
					break;
			}

			if(!connection->checkResult(result, "SQLExecute")) {
				if(parameterBindings.empty()) {
					log(LL_Error, "DB query %s failed at input %d: %s\n", queryStr.c_str(), (int) i, statusStr);
				} else {
					log(LL_Error,
					    "DB query %s: %s\nParameters:\n%s",
					    queryStr.c_str(),
					    statusStr,
					    logParameters(queryJob->getInputPointer(i)).c_str());
				}
			}
		}
	}

	connection->closeCursor();
	connection->endTransaction(true);
	connection->release();

	return true;
}

void DbQueryBinding::setString(DbConnection* connection,
                               const ParameterBinding& paramBinding,
                               SQLLEN* StrLen_or_Ind,
                               const std::string& str,
                               std::string& outStr) {
	CharsetConverter localToUtf16(CharsetConverter::getEncoding().c_str(), "UTF-16LE");
	localToUtf16.convert(str, outStr, 2);

	// If the string is empty, put a size of 1 else SQL server complains about invalid precision
	connection->bindParameter(*paramBinding.index,
	                          SQL_PARAM_INPUT,
	                          SQL_C_WCHAR,
	                          SQL_WVARCHAR,
	                          str.size() > 0 ? str.size() : 1,
	                          paramBinding.dbPrecision,
	                          (SQLPOINTER) outStr.data(),
	                          0,
	                          StrLen_or_Ind);
}

bool DbQueryBinding::getString(DbConnection* connection, int columnIndex, std::string* outString) {
	std::string unicodeBuffer;
	SQLLEN bytesRead = 0;
	SQLLEN dataSize;
	SQLLEN isDataNull;
	int dummy;
	SQLRETURN ret;

	if(!connection->getData(columnIndex, SQL_C_BINARY, &dummy, 0, &dataSize, true))
		return false;

	if(dataSize == SQL_NULL_DATA || dataSize == 0) {
		outString->clear();
		return true;
	} else if(dataSize < 0) {
		logStatic(LL_Warning,
		          DbQueryBinding::getStaticClassName(),
		          "getString: dataSize is negative: %" PRId64 "\n",
		          (int64_t) dataSize);
		return false;
	}

	unicodeBuffer.resize(dataSize * 2 + 4);

	while(connection->getData(columnIndex,
	                          SQL_C_WCHAR,
	                          &unicodeBuffer[bytesRead],
	                          unicodeBuffer.size() - bytesRead,
	                          &isDataNull,
	                          false,
	                          &ret) &&
	      ret == SQL_SUCCESS_WITH_INFO) {
		if(isDataNull == SQL_NULL_DATA)
			break;

		bytesRead = unicodeBuffer.size() - 2;  // dont keep null terminator
		unicodeBuffer.resize(unicodeBuffer.size() * 2);
	}

	if(isDataNull == SQL_NULL_DATA || ret == SQL_NO_DATA) {
		outString->clear();
		return true;
	} else if(isDataNull < 0) {
		logStatic(LL_Warning,
		          DbQueryBinding::getStaticClassName(),
		          "getString: isDataNull is negative: %" PRId64 ", status: %d\n",
		          (int64_t) isDataNull,
		          ret);
		return false;
	} else if(ret == SQL_SUCCESS) {
		bytesRead += isDataNull;
		unicodeBuffer.resize(bytesRead);

		if(bytesRead != 0) {
			CharsetConverter utf16ToLocal("UTF-16LE", CharsetConverter::getEncoding().c_str());
			utf16ToLocal.convert(unicodeBuffer, *outString, 0.5);
		} else {
			outString->clear();
		}
		return true;
	}

	outString->clear();
	return false;
}
