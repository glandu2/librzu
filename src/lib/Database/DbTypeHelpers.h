#ifndef DBTYPEHELPERS_H
#define DBTYPEHELPERS_H

#include "../Extern.h"
#include <sstream>
#include <string>
#include <uv.h>

#include <sqlext.h>

template<size_t N> class DbString;

struct RZU_EXTERN DbDateTime : public SQL_TIMESTAMP_STRUCT {
	void setUnixTime(time_t t, uint32_t nanoFraction = 0);
};

template<typename FieldType>
struct IfValidPrintableType : public std::integral_constant<bool,
                                                            std::is_fundamental<FieldType>::value ||
                                                                std::is_same<FieldType, std::string>::value> {};

template<typename T, typename A = void> struct DbPrintableTypeBinding {};

template<typename T>
struct DbPrintableTypeBinding<T, typename std::enable_if<IfValidPrintableType<T>::value, void>::type> {
	static void print(std::ostringstream& stream, void* data) { stream << *(reinterpret_cast<T*>(data)); }
};
template<size_t N> struct DbPrintableTypeBinding<DbString<N>, void> {
	static void print(std::ostringstream& stream, void* data) {
		const DbString<N>* strData = reinterpret_cast<const DbString<N>*>(data);
		stream << strData->c_str();
	}
};
template<size_t ARRAY_SIZE> struct DbPrintableTypeBinding<char[ARRAY_SIZE], void> {
	static void print(std::ostringstream& stream, void* data) { stream << "complex data"; }
};

template<> struct DbPrintableTypeBinding<char> {
	static void print(std::ostringstream& stream, void* data) { stream << (int) (*(reinterpret_cast<char*>(data))); }
};
template<> struct DbPrintableTypeBinding<signed char> : public DbPrintableTypeBinding<char> {};
template<> struct DbPrintableTypeBinding<unsigned char> : public DbPrintableTypeBinding<char> {};

template<> struct RZU_EXTERN DbPrintableTypeBinding<SQL_TIMESTAMP_STRUCT> {
	static void print(std::ostringstream& stream, void* data);
};
template<> struct DbPrintableTypeBinding<DbDateTime> : public DbPrintableTypeBinding<SQL_TIMESTAMP_STRUCT> {};

template<typename T> struct DbTypeBinding {};

template<> struct DbTypeBinding<bool> {
	enum { C_TYPE = SQL_C_BIT, SQL_TYPE = SQL_BIT, SQL_SIZE = 1, SQL_PRECISION = 0 };
};
template<> struct DbTypeBinding<char> {
	enum { C_TYPE = SQL_C_TINYINT, SQL_TYPE = SQL_TINYINT, SQL_SIZE = 3, SQL_PRECISION = 0 };
};
template<> struct DbTypeBinding<int8_t> {
	enum { C_TYPE = SQL_C_STINYINT, SQL_TYPE = SQL_TINYINT, SQL_SIZE = 3, SQL_PRECISION = 0 };
};
template<> struct DbTypeBinding<uint8_t> {
	enum { C_TYPE = SQL_C_UTINYINT, SQL_TYPE = SQL_TINYINT, SQL_SIZE = 3, SQL_PRECISION = 0 };
};
template<> struct DbTypeBinding<int16_t> {
	enum { C_TYPE = SQL_C_SHORT, SQL_TYPE = SQL_SMALLINT, SQL_SIZE = 5, SQL_PRECISION = 0 };
};
template<> struct DbTypeBinding<uint16_t> {
	enum { C_TYPE = SQL_C_USHORT, SQL_TYPE = SQL_SMALLINT, SQL_SIZE = 5, SQL_PRECISION = 0 };
};
template<> struct DbTypeBinding<int32_t> {
	enum { C_TYPE = SQL_C_LONG, SQL_TYPE = SQL_INTEGER, SQL_SIZE = 10, SQL_PRECISION = 0 };
};
template<> struct DbTypeBinding<uint32_t> {
	enum { C_TYPE = SQL_C_ULONG, SQL_TYPE = SQL_INTEGER, SQL_SIZE = 10, SQL_PRECISION = 0 };
};
template<> struct DbTypeBinding<int64_t> {
	enum { C_TYPE = SQL_C_SBIGINT, SQL_TYPE = SQL_BIGINT, SQL_SIZE = 19, SQL_PRECISION = 0 };
};
template<> struct DbTypeBinding<uint64_t> {
	enum { C_TYPE = SQL_C_UBIGINT, SQL_TYPE = SQL_BIGINT, SQL_SIZE = 19, SQL_PRECISION = 0 };
};
template<> struct DbTypeBinding<float> {
	enum { C_TYPE = SQL_C_FLOAT, SQL_TYPE = SQL_REAL, SQL_SIZE = 7, SQL_PRECISION = 0 };
};
template<> struct DbTypeBinding<double> {
	enum { C_TYPE = SQL_C_DOUBLE, SQL_TYPE = SQL_DOUBLE, SQL_SIZE = 15, SQL_PRECISION = 0 };
};

template<> struct DbTypeBinding<SQL_TIMESTAMP_STRUCT> {
	enum { C_TYPE = SQL_C_TIMESTAMP, SQL_TYPE = SQL_TYPE_TIMESTAMP, SQL_SIZE = 23, SQL_PRECISION = 3 };
};
template<> struct DbTypeBinding<DbDateTime> : public DbTypeBinding<SQL_TIMESTAMP_STRUCT> {};

template<int ARRAY_SIZE> struct DbTypeBinding<char[ARRAY_SIZE]> {
	enum { C_TYPE = SQL_C_CHAR, SQL_TYPE = SQL_VARCHAR, SQL_SIZE = ARRAY_SIZE, SQL_PRECISION = 0 };
};
template<> struct DbTypeBinding<std::string> {
	enum { C_TYPE = SQL_C_WCHAR, SQL_TYPE = SQL_WVARCHAR, SQL_SIZE = -1, SQL_PRECISION = 0 };
};
template<size_t N> struct DbTypeBinding<DbString<N>> {
	enum { C_TYPE = SQL_C_WCHAR, SQL_TYPE = SQL_WVARCHAR, SQL_SIZE = -1, SQL_PRECISION = 0 };
};

template<int ARRAY_SIZE>
struct DbTypeBinding<unsigned char[ARRAY_SIZE]> : public DbTypeBinding<char (&)[ARRAY_SIZE]> {};

enum DbValueType { DVT_Primitive, DVT_StdString, DVT_DbString };

template<typename T> struct IsStdString { static constexpr DbValueType value = DVT_Primitive; };
template<> struct IsStdString<std::string> { static constexpr DbValueType value = DVT_StdString; };
template<size_t N> struct IsStdString<DbString<N>> { static constexpr DbValueType value = DVT_DbString; };

#endif  // DBTYPEHELPERS_H
