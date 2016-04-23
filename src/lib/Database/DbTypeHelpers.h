#ifndef DBTYPEHELPERS_H
#define DBTYPEHELPERS_H

#include <uv.h>
#include <sqlext.h>
#include <string>
#include "Extern.h"

struct RZU_EXTERN DbDateTime : public SQL_TIMESTAMP_STRUCT {
	void setUnixTime(time_t t, uint32_t nanoFraction = 0);
};

template<typename T> struct DbTypeBinding {};

template<> struct DbTypeBinding<bool> { enum { C_TYPE = SQL_C_BIT, SQL_TYPE = SQL_BIT, SQL_SIZE = 1, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<char> { enum { C_TYPE = SQL_C_TINYINT, SQL_TYPE = SQL_TINYINT, SQL_SIZE = 3, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<signed char> { enum { C_TYPE = SQL_C_TINYINT, SQL_TYPE = SQL_TINYINT, SQL_SIZE = 3, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<short> { enum { C_TYPE = SQL_C_SHORT, SQL_TYPE = SQL_SMALLINT, SQL_SIZE = 5, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<int> { enum { C_TYPE = SQL_C_LONG, SQL_TYPE = SQL_INTEGER, SQL_SIZE = 10, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<long long> { enum { C_TYPE = SQL_C_SBIGINT, SQL_TYPE = SQL_BIGINT, SQL_SIZE = 19, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<float> { enum { C_TYPE = SQL_C_FLOAT, SQL_TYPE = SQL_REAL, SQL_SIZE = 7, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<double> { enum { C_TYPE = SQL_C_DOUBLE, SQL_TYPE = SQL_DOUBLE, SQL_SIZE = 15, SQL_PRECISION = 0 }; };

template<> struct DbTypeBinding<SQL_TIMESTAMP_STRUCT> {enum { C_TYPE = SQL_C_TIMESTAMP, SQL_TYPE = SQL_TYPE_TIMESTAMP, SQL_SIZE = 23, SQL_PRECISION = 3 }; };
template<> struct DbTypeBinding<DbDateTime> : DbTypeBinding<SQL_TIMESTAMP_STRUCT> {};

template<> struct DbTypeBinding<unsigned char> : DbTypeBinding<char> {};
template<> struct DbTypeBinding<unsigned short> : DbTypeBinding<short> {};
template<> struct DbTypeBinding<unsigned int> : DbTypeBinding<int> {};
template<> struct DbTypeBinding<long> : DbTypeBinding<long long> {};
template<> struct DbTypeBinding<unsigned long long> : DbTypeBinding<long long> {};
template<> struct DbTypeBinding<unsigned long> : DbTypeBinding<unsigned long long> {};

template<int ARRAY_SIZE> struct DbTypeBinding<char[ARRAY_SIZE]> { enum { C_TYPE = SQL_C_CHAR, SQL_TYPE = SQL_VARCHAR, SQL_SIZE = ARRAY_SIZE, SQL_PRECISION = 0 }; };
template<> struct DbTypeBinding<std::string> { enum { C_TYPE = SQL_C_WCHAR, SQL_TYPE = SQL_WVARCHAR, SQL_SIZE = -1, SQL_PRECISION = 0 }; };

template<int ARRAY_SIZE> struct DbTypeBinding<unsigned char[ARRAY_SIZE]> : DbTypeBinding<char(&)[ARRAY_SIZE]> { };

template<typename T> struct IsStdString { enum { value = false }; };
template<> struct IsStdString<std::string> { enum { value = true }; };

#endif // DBTYPEHELPERS_H
