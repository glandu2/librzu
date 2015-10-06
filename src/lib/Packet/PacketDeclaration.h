#ifndef PACKETDECLARATION_H
#define PACKETDECLARATION_H

#include <string.h>
#include <stdint.h>
#include <vector>
#include <string>
#include "PacketEpics.h"
#include "PacketBaseMessage.h"

#define _ARG5(_0, _1, _2, _3, _4, _5, ...) _5
#define _EXPAND(x) x
#define NARG5(...) _EXPAND(_ARG5(__VA_ARGS__, 5, 4, 3, 2, 1, 0))

#define __ONE_OR_TWO_ARGS(MACRO, N, ...) _EXPAND(MACRO ## N (__VA_ARGS__))
#define _ONE_OR_TWO_ARGS(MACRO, N, ...) __ONE_OR_TWO_ARGS(MACRO, N, __VA_ARGS__)
#define OVERLOADED_CALL(macro, ...) _ONE_OR_TWO_ARGS(macro, NARG5(__VA_ARGS__), __VA_ARGS__)

// Make types highlight in qtcreator
#define int8_t int8_t
#define int16_t int16_t
#define int32_t int32_t
#define int64_t int64_t

#define uint8_t uint8_t
#define uint16_t uint16_t
#define uint32_t uint32_t
#define uint64_t uint64_t

namespace PacketDeclaration {

template<typename T>
inline void copyDefaultValue(T& val1, bool val2) {
	val1 = val2;
}

template<typename T>
inline void copyDefaultValue(T& val1, int val2) {
	val1 = val2;
}

template<typename T>
inline void copyDefaultValue(T& val1, float val2) {
	val1 = val2;
}

template<typename T>
inline void copyDefaultValue(T& val1, double val2) {
	val1 = val2;
}

template<typename T>
inline void copyDefaultValue(T val1[], const T val2[], size_t size) {
	memcpy(val1, val2, size);
}

template<class T>
inline
typename std::enable_if<!std::is_fundamental<T>::value && !std::is_enum<T>::value, int>::type
getSizeOf(const T& value, int version) {
	return value.getSize(version);
}

template<typename T>
inline
typename std::enable_if<std::is_fundamental<T>::value || std::is_enum<T>::value, int>::type
getSizeOf(const T& value, int version) {
	return sizeof(value);
}

} // namespace PacketDeclaration

// Body fields
#define DEFINITION_F_SIMPLE(...) OVERLOADED_CALL(DEFINITION_F_SIMPLE, __VA_ARGS__)
#define DEFINITION_F_ARRAY(...) OVERLOADED_CALL(DEFINITION_F_ARRAY, __VA_ARGS__)
#define DEFINITION_F_DYNARRAY(...) OVERLOADED_CALL(DEFINITION_F_DYNARRAY, __VA_ARGS__)
#define DEFINITION_F_COUNT(...) OVERLOADED_CALL(DEFINITION_F_COUNT, __VA_ARGS__)
#define DEFINITION_F_STRING(...) OVERLOADED_CALL(DEFINITION_F_STRING, __VA_ARGS__)
#define DEFINITION_F_DYNSTRING(...) OVERLOADED_CALL(DEFINITION_F_DYNSTRING, __VA_ARGS__)

#define DEFINITION_F_SIMPLE2(type, name) type name;
#define DEFINITION_F_SIMPLE3(type, name, cond) type name;
#define DEFINITION_F_SIMPLE4(type, name, cond, defaultval) type name;

#define DEFINITION_F_ARRAY3(type, name, size) type name[size];
#define DEFINITION_F_ARRAY4(type, name, size, cond) type name[size];
#define DEFINITION_F_ARRAY5(type, name, size, cond, defaultval) type name[size];

#define DEFINITION_F_DYNARRAY2(type, name) std::vector<type> name;
#define DEFINITION_F_DYNARRAY3(type, name, cond) std::vector<type> name;
#define DEFINITION_F_DYNARRAY4(type, name, cond, defaultval) std::vector<type> name;

#define DEFINITION_F_COUNT3(type, name, ref)
#define DEFINITION_F_COUNT4(type, name, ref, cond)
#define DEFINITION_F_COUNT5(type, name, ref, cond, defaultval)

#define DEFINITION_F_STRING2(name, size) std::string name;
#define DEFINITION_F_STRING3(name, size, cond) std::string name;
#define DEFINITION_F_STRING4(name, size, cond, defaultval) std::string name;

#define DEFINITION_F_DYNSTRING2(name, hasNullTerminator) std::string name;
#define DEFINITION_F_DYNSTRING3(name, hasNullTerminator, cond) std::string name;
#define DEFINITION_F_DYNSTRING4(name, hasNullTerminator, cond, defaultval) std::string name;

// Size function
#define SIZE_F_SIMPLE(...) OVERLOADED_CALL(SIZE_F_SIMPLE, __VA_ARGS__)
#define SIZE_F_ARRAY(...) OVERLOADED_CALL(SIZE_F_ARRAY, __VA_ARGS__)
#define SIZE_F_DYNARRAY(...) OVERLOADED_CALL(SIZE_F_DYNARRAY, __VA_ARGS__)
#define SIZE_F_COUNT(...) OVERLOADED_CALL(SIZE_F_COUNT, __VA_ARGS__)
#define SIZE_F_STRING(...) OVERLOADED_CALL(SIZE_F_STRING, __VA_ARGS__)
#define SIZE_F_DYNSTRING(...) OVERLOADED_CALL(SIZE_F_DYNSTRING, __VA_ARGS__)

#define SIZE_F_SIMPLE2(type, name) \
	size += PacketDeclaration::getSizeOf((type)name, version);
#define SIZE_F_SIMPLE3(type, name, cond) \
	if(cond) size += PacketDeclaration::getSizeOf((type)name, version);
#define SIZE_F_SIMPLE4(type, name, cond, defaultval) \
	if(cond) size += PacketDeclaration::getSizeOf((type)name, version);

#define SIZE_F_ARRAY3(type, name, _size) \
	for(int i = 0; i < _size; ++i) size += PacketDeclaration::getSizeOf((type)name[i], version);
#define SIZE_F_ARRAY4(type, name, _size, cond) \
	if(cond) for(int i = 0; i < _size; ++i) size += PacketDeclaration::getSizeOf((type)name[i], version);
#define SIZE_F_ARRAY5(type, name, _size, cond, defaultval) \
	if(cond) for(int i = 0; i < _size; ++i) size += PacketDeclaration::getSizeOf((type)name[i], version);

#define SIZE_F_DYNARRAY2(type, name) \
	for(auto it = name.begin(); it != name.end(); ++it) size += PacketDeclaration::getSizeOf((type)*it, version);
#define SIZE_F_DYNARRAY3(type, name, cond) \
	if(cond) for(auto it = name.begin(); it != name.end(); ++it) size += PacketDeclaration::getSizeOf((type)*it, version);
#define SIZE_F_DYNARRAY4(type, name, cond, defaultval) \
	if(cond) for(auto it = name.begin(); it != name.end(); ++it) size += PacketDeclaration::getSizeOf((type)*it, version);

#define SIZE_F_COUNT3(type, name, ref) \
	size += sizeof(type);
#define SIZE_F_COUNT4(type, name, ref, cond) \
	if(cond) size += sizeof(type);
#define SIZE_F_COUNT5(type, name, ref, cond, defaultval) \
	if(cond) size += sizeof(type);

#define SIZE_F_STRING2(name, _size) \
	size += _size;
#define SIZE_F_STRING3(name, _size, cond) \
	if(cond) size += _size;
#define SIZE_F_STRING4(name, _size, cond, defaultval) \
	if(cond) size += _size;

#define SIZE_F_DYNSTRING2(name, hasNullTerminator) \
	size += (uint32_t)name.size() + hasNullTerminator;
#define SIZE_F_DYNSTRING3(name, hasNullTerminator, cond) \
	if(cond) size += (uint32_t)name.size() + hasNullTerminator;
#define SIZE_F_DYNSTRING4(name, hasNullTerminator, cond, defaultval) \
	if(cond) size += (uint32_t)name.size() + hasNullTerminator;

// Serialization function
#define SERIALIZATION_F_SIMPLE(...) OVERLOADED_CALL(SERIALIZATION_F_SIMPLE, __VA_ARGS__)
#define SERIALIZATION_F_ARRAY(...) OVERLOADED_CALL(SERIALIZATION_F_ARRAY, __VA_ARGS__)
#define SERIALIZATION_F_DYNARRAY(...) OVERLOADED_CALL(SERIALIZATION_F_DYNARRAY, __VA_ARGS__)
#define SERIALIZATION_F_COUNT(...) OVERLOADED_CALL(SERIALIZATION_F_COUNT, __VA_ARGS__)
#define SERIALIZATION_F_STRING(...) OVERLOADED_CALL(SERIALIZATION_F_STRING, __VA_ARGS__)
#define SERIALIZATION_F_DYNSTRING(...) OVERLOADED_CALL(SERIALIZATION_F_DYNSTRING, __VA_ARGS__)

#define SERIALIZATION_F_SIMPLE2(type, name) \
	buffer->write(#name, (type)name);
#define SERIALIZATION_F_SIMPLE3(type, name, cond) \
	if(cond) buffer->write(#name, (type)name);
#define SERIALIZATION_F_SIMPLE4(type, name, cond, defaultval) \
	if(cond) buffer->write(#name, (type)name);

#define SERIALIZATION_F_ARRAY3(type, name, size) \
	buffer->writeArray(#name, (type*)name, size);
#define SERIALIZATION_F_ARRAY4(type, name, size, cond) \
	if(cond) buffer->writeArray(#name, (type*)name, size);
#define SERIALIZATION_F_ARRAY5(type, name, size, cond, defaultval) \
	if(cond) buffer->writeArray(#name, (type*)name, size);

#define SERIALIZATION_F_DYNARRAY2(type, name) \
	buffer->template writeDynArray<type>(#name, name);
#define SERIALIZATION_F_DYNARRAY3(type, name, cond) \
	if(cond) buffer->template writeDynArray<type>(#name, name);
#define SERIALIZATION_F_DYNARRAY4(type, name, cond, defaultval) \
	if(cond) buffer->template writeDynArray<type>(#name, name);

#define SERIALIZATION_F_COUNT3(type, name, ref) \
	buffer->write(#name, (type)ref.size());
#define SERIALIZATION_F_COUNT4(type, name, ref, cond) \
	if(cond) buffer->write(#name, (type)ref.size());
#define SERIALIZATION_F_COUNT5(type, name, ref, cond, defaultval) \
	if(cond) buffer->write(#name, (type)ref.size());

#define SERIALIZATION_F_STRING2(name, size) \
	buffer->writeString(#name, name, size);
#define SERIALIZATION_F_STRING3(name, size, cond) \
	if(cond) buffer->writeString(#name, name, size);
#define SERIALIZATION_F_STRING4(name, size, cond, defaultval) \
	if(cond) buffer->writeString(#name, name, size);

#define SERIALIZATION_F_DYNSTRING2(name, hasNullTerminator) \
	buffer->writeDynString(#name, name, hasNullTerminator);
#define SERIALIZATION_F_DYNSTRING3(name, hasNullTerminator, cond) \
	if(cond) buffer->writeDynString(#name, name, hasNullTerminator);
#define SERIALIZATION_F_DYNSTRING4(name, hasNullTerminator, cond, defaultval) \
	if(cond) buffer->writeDynString(#name, name, hasNullTerminator);

// Deserialization function
#define DESERIALIZATION_F_SIMPLE(...) OVERLOADED_CALL(DESERIALIZATION_F_SIMPLE, __VA_ARGS__)
#define DESERIALIZATION_F_ARRAY(...) OVERLOADED_CALL(DESERIALIZATION_F_ARRAY, __VA_ARGS__)
#define DESERIALIZATION_F_DYNARRAY(...) OVERLOADED_CALL(DESERIALIZATION_F_DYNARRAY, __VA_ARGS__)
#define DESERIALIZATION_F_COUNT(...) OVERLOADED_CALL(DESERIALIZATION_F_COUNT, __VA_ARGS__)
#define DESERIALIZATION_F_STRING(...) OVERLOADED_CALL(DESERIALIZATION_F_STRING, __VA_ARGS__)
#define DESERIALIZATION_F_DYNSTRING(...) OVERLOADED_CALL(DESERIALIZATION_F_DYNSTRING, __VA_ARGS__)

#define DESERIALIZATION_F_SIMPLE2(type, name) \
	buffer->template read<type>(#name, name);
#define DESERIALIZATION_F_SIMPLE3(type, name, cond) \
	if(cond) buffer->template read<type>(#name, name);
#define DESERIALIZATION_F_SIMPLE4(type, name, cond, defaultval) \
	if(cond) buffer->template read<type>(#name, name); else PacketDeclaration::copyDefaultValue(name, defaultval);

#define DESERIALIZATION_F_ARRAY3(type, name, size) \
	buffer->template readArray<type>(#name, name, size);
#define DESERIALIZATION_F_ARRAY4(type, name, size, cond) \
	if(cond) buffer->template readArray<type>(#name, name, size);
#define DESERIALIZATION_F_ARRAY5(type, name, size, cond, defaultval) \
	if(cond) \
		buffer->template readArray<type>(#name, name, size); \
	else { \
		static const type defaultArray[size] = defaultval; \
		PacketDeclaration::copyDefaultValue(name, defaultArray, size); \
	}

#define DESERIALIZATION_F_DYNARRAY2(type, name) \
	buffer->template readDynArray<type>(#name, name);
#define DESERIALIZATION_F_DYNARRAY3(type, name, cond) \
	if(cond) buffer->template readDynArray<type>(#name, name);
#define DESERIALIZATION_F_DYNARRAY4(type, name, cond, defaultval) \
	if(cond) buffer->template readDynArray<type>(#name, name); else PacketDeclaration::copyDefaultValue(name, defaultval);

#define DESERIALIZATION_F_COUNT3(type, name, ref) \
	buffer->template readSize<type>(#name, ref);
#define DESERIALIZATION_F_COUNT4(type, name, ref, cond) \
	if(cond) buffer->template readSize<type>(#name, ref);
#define DESERIALIZATION_F_COUNT5(type, name, ref, cond, defaultval) \
	if(cond) buffer->template readSize<type>(#name, ref); else PacketDeclaration::copyDefaultValue(name, defaultval);

#define DESERIALIZATION_F_STRING2(name, size) \
	buffer->readString(#name, name, size);
#define DESERIALIZATION_F_STRING3(name, size, cond) \
	if(cond) buffer->readString(#name, name, size);
#define DESERIALIZATION_F_STRING4(name, size, cond, defaultval) \
	if(cond) \
		buffer->readString(#name, name, size); \
	else \
		name = defaultval;

#define DESERIALIZATION_F_DYNSTRING2(name, hasNullTerminator) \
	buffer->readDynString(#name, name, hasNullTerminator);
#define DESERIALIZATION_F_DYNSTRING3(name, hasNullTerminator, cond) \
	if(cond) buffer->readDynString(#name, name, hasNullTerminator);
#define DESERIALIZATION_F_DYNSTRING4(name, hasNullTerminator, cond, defaultval) \
	if(cond) \
		buffer->readDynString(#name, name, hasNullTerminator); \
	else \
		name = defaultval;


// def / impl mode implementation
// needed to bypass recursion limitation
#define DEFINITION_F_SIMPLE_(...) OVERLOADED_CALL(DEFINITION_F_SIMPLE, __VA_ARGS__)
#define DEFINITION_F_ARRAY_(...) OVERLOADED_CALL(DEFINITION_F_ARRAY, __VA_ARGS__)
#define DEFINITION_F_DYNARRAY_(...) OVERLOADED_CALL(DEFINITION_F_DYNARRAY, __VA_ARGS__)
#define DEFINITION_F_COUNT_(...) OVERLOADED_CALL(DEFINITION_F_COUNT, __VA_ARGS__)
#define DEFINITION_F_STRING_(...) OVERLOADED_CALL(DEFINITION_F_STRING, __VA_ARGS__)
#define SERIALIZATION_F_SIMPLE_(...) OVERLOADED_CALL(SERIALIZATION_F_SIMPLE, __VA_ARGS__)
#define SERIALIZATION_F_ARRAY_(...) OVERLOADED_CALL(SERIALIZATION_F_ARRAY, __VA_ARGS__)
#define SERIALIZATION_F_DYNARRAY_(...) OVERLOADED_CALL(SERIALIZATION_F_DYNARRAY, __VA_ARGS__)
#define SERIALIZATION_F_COUNT_(...) OVERLOADED_CALL(SERIALIZATION_F_COUNT, __VA_ARGS__)
#define SERIALIZATION_F_STRING_(...) OVERLOADED_CALL(SERIALIZATION_F_STRING, __VA_ARGS__)
#define DESERIALIZATION_F_SIMPLE_(...) OVERLOADED_CALL(DESERIALIZATION_F_SIMPLE, __VA_ARGS__)
#define DESERIALIZATION_F_ARRAY_(...) OVERLOADED_CALL(DESERIALIZATION_F_ARRAY, __VA_ARGS__)
#define DESERIALIZATION_F_DYNARRAY_(...) OVERLOADED_CALL(DESERIALIZATION_F_DYNARRAY, __VA_ARGS__)
#define DESERIALIZATION_F_COUNT_(...) OVERLOADED_CALL(DESERIALIZATION_F_COUNT, __VA_ARGS__)
#define DESERIALIZATION_F_STRING_(...) OVERLOADED_CALL(DESERIALIZATION_F_STRING, __VA_ARGS__)
#define SIZE_F_SIMPLE_(...) OVERLOADED_CALL(SIZE_F_SIMPLE, __VA_ARGS__)
#define SIZE_F_ARRAY_(...) OVERLOADED_CALL(SIZE_F_ARRAY, __VA_ARGS__)
#define SIZE_F_DYNARRAY_(...) OVERLOADED_CALL(SIZE_F_DYNARRAY, __VA_ARGS__)
#define SIZE_F_COUNT_(...) OVERLOADED_CALL(SIZE_F_COUNT, __VA_ARGS__)
#define SIZE_F_STRING_(...) OVERLOADED_CALL(SIZE_F_STRING, __VA_ARGS__)

#define DO_NOTHING(...)

#define DEFINITION_def(x) x
#define SIZE_def(x) DO_NOTHING
#define SERIALIZATION_def(x) DO_NOTHING
#define DESERIALIZATION_def(x) DO_NOTHING

#define DEFINITION_impl(x) DO_NOTHING
#define SIZE_impl(x) x
#define SERIALIZATION_impl(x) x
#define DESERIALIZATION_impl(x) x

#define DEFINITION_F_SIMPLE1(mode) DEFINITION_ ## mode (DEFINITION_F_SIMPLE_)
#define DEFINITION_F_ARRAY1(mode) DEFINITION_ ## mode (DEFINITION_F_ARRAY_)
#define DEFINITION_F_DYNARRAY1(mode) DEFINITION_ ## mode (DEFINITION_F_DYNARRAY_)
#define DEFINITION_F_COUNT1(mode) DEFINITION_ ## mode (DEFINITION_F_COUNT_)
#define DEFINITION_F_STRING1(mode) DEFINITION_ ## mode (DEFINITION_F_STRING_)

#define SIZE_F_SIMPLE1(mode) SIZE_ ## mode (SIZE_F_SIMPLE_)
#define SIZE_F_ARRAY1(mode) SIZE_ ## mode (SIZE_F_ARRAY_)
#define SIZE_F_DYNARRAY1(mode) SIZE_ ## mode (SIZE_F_DYNARRAY_)
#define SIZE_F_COUNT1(mode) SIZE_ ## mode (SIZE_F_COUNT_)
#define SIZE_F_STRING1(mode) SIZE_ ## mode (SIZE_F_STRING_)

#define SERIALIZATION_F_SIMPLE1(mode) SERIALIZATION_ ## mode (SERIALIZATION_F_SIMPLE_)
#define SERIALIZATION_F_ARRAY1(mode) SERIALIZATION_ ## mode (SERIALIZATION_F_ARRAY_)
#define SERIALIZATION_F_DYNARRAY1(mode) SERIALIZATION_ ## mode (SERIALIZATION_F_DYNARRAY_)
#define SERIALIZATION_F_COUNT1(mode) SERIALIZATION_ ## mode (SERIALIZATION_F_COUNT_)
#define SERIALIZATION_F_STRING1(mode) SERIALIZATION_ ## mode (SERIALIZATION_F_STRING_)

#define DESERIALIZATION_F_SIMPLE1(mode) DESERIALIZATION_ ## mode (DESERIALIZATION_F_SIMPLE_)
#define DESERIALIZATION_F_ARRAY1(mode) DESERIALIZATION_ ## mode (DESERIALIZATION_F_ARRAY_)
#define DESERIALIZATION_F_DYNARRAY1(mode) DESERIALIZATION_ ## mode (DESERIALIZATION_F_DYNARRAY_)
#define DESERIALIZATION_F_COUNT1(mode) DESERIALIZATION_ ## mode (DESERIALIZATION_F_COUNT_)
#define DESERIALIZATION_F_STRING1(mode) DESERIALIZATION_ ## mode (DESERIALIZATION_F_STRING_)

#define CREATE_STRUCT(name) \
	struct name { \
		name ## _DEF(DEFINITION_F_SIMPLE, DEFINITION_F_ARRAY, DEFINITION_F_DYNARRAY, DEFINITION_F_COUNT, DEFINITION_F_STRING, DEFINITION_F_DYNSTRING) \
	\
		uint32_t getSize(int version) const { \
			uint32_t size = 0; \
			name ## _DEF(SIZE_F_SIMPLE, SIZE_F_ARRAY, SIZE_F_DYNARRAY, SIZE_F_COUNT, SIZE_F_STRING, SIZE_F_DYNSTRING) \
	\
			return size;\
		} \
	\
		template<class T> \
		void serialize(T* buffer) const { \
			const int version = buffer->getVersion(); \
			(void)(version); \
			name ## _DEF(SERIALIZATION_F_SIMPLE, SERIALIZATION_F_ARRAY, SERIALIZATION_F_DYNARRAY, SERIALIZATION_F_COUNT, SERIALIZATION_F_STRING, SERIALIZATION_F_DYNSTRING) \
		} \
	\
		template<class T> \
		void deserialize(T* buffer) { \
			const int version = buffer->getVersion(); \
			(void)(version); \
	\
			name ## _DEF(DESERIALIZATION_F_SIMPLE, DESERIALIZATION_F_ARRAY, DESERIALIZATION_F_DYNARRAY, DESERIALIZATION_F_COUNT, DESERIALIZATION_F_STRING, DESERIALIZATION_F_DYNSTRING) \
		} \
	}

#define CREATE_PACKET(name, id) \
	struct name { \
		static const uint16_t packetID = id; \
	\
		name ## _DEF(DEFINITION_F_SIMPLE, DEFINITION_F_ARRAY, DEFINITION_F_DYNARRAY, DEFINITION_F_COUNT, DEFINITION_F_STRING, DEFINITION_F_DYNSTRING) \
	\
		uint32_t getSize(int version) const { \
			uint32_t size = 7; \
			name ## _DEF(SIZE_F_SIMPLE, SIZE_F_ARRAY, SIZE_F_DYNARRAY, SIZE_F_COUNT, SIZE_F_STRING, SIZE_F_DYNSTRING) \
	\
			return size;\
		} \
	\
		template<class T> \
		void serialize(T* buffer) const { \
			const int version = buffer->getVersion(); \
			(void)(version); \
			uint32_t size = getSize(buffer->getVersion()); \
			buffer->write("size", size); \
			buffer->write("id", packetID); \
			buffer->write("msg_checksum", getMessageChecksum(size, packetID)); \
			\
			name ## _DEF(SERIALIZATION_F_SIMPLE, SERIALIZATION_F_ARRAY, SERIALIZATION_F_DYNARRAY, SERIALIZATION_F_COUNT, SERIALIZATION_F_STRING, SERIALIZATION_F_DYNSTRING) \
		} \
	\
		template<class T> \
		void deserialize(T* buffer) { \
			const int version = buffer->getVersion(); \
			(void)(version); \
			buffer->discard("size", 4); \
			buffer->discard("id", 2); \
			buffer->discard("msg_checksum", 1); \
			\
			name ## _DEF(DESERIALIZATION_F_SIMPLE, DESERIALIZATION_F_ARRAY, DESERIALIZATION_F_DYNARRAY, DESERIALIZATION_F_COUNT, DESERIALIZATION_F_STRING, DESERIALIZATION_F_DYNSTRING) \
		} \
	}

#endif // PACKETDECLARATION_H

