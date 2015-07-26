#ifndef PACKETDECLARATION_H
#define PACKETDECLARATION_H

#include <string.h>
#include <stdint.h>
#include <vector>
#include "Epics.h"

#ifndef TEST_PACKET
#include "PacketBaseMessage.h"
#endif

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

inline void copyDefaultValue(char val1[], const char val2[]) {
	strcpy(val1, val2);
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

// Size function
#define SIZE_F_SIMPLE(...) OVERLOADED_CALL(SIZE_F_SIMPLE, __VA_ARGS__)
#define SIZE_F_ARRAY(...) OVERLOADED_CALL(SIZE_F_ARRAY, __VA_ARGS__)
#define SIZE_F_DYNARRAY(...) OVERLOADED_CALL(SIZE_F_DYNARRAY, __VA_ARGS__)
#define SIZE_F_COUNT(...) OVERLOADED_CALL(SIZE_F_COUNT, __VA_ARGS__)

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

// Serialization function
#define SERIALIZATION_F_SIMPLE(...) OVERLOADED_CALL(SERIALIZATION_F_SIMPLE, __VA_ARGS__)
#define SERIALIZATION_F_ARRAY(...) OVERLOADED_CALL(SERIALIZATION_F_ARRAY, __VA_ARGS__)
#define SERIALIZATION_F_DYNARRAY(...) OVERLOADED_CALL(SERIALIZATION_F_DYNARRAY, __VA_ARGS__)
#define SERIALIZATION_F_COUNT(...) OVERLOADED_CALL(SERIALIZATION_F_COUNT, __VA_ARGS__)

#define SERIALIZATION_F_SIMPLE2(type, name) \
	buffer->write(#name, (type)name);
#define SERIALIZATION_F_SIMPLE3(type, name, cond) \
	if(cond) buffer->write(#name, (type)name);
#define SERIALIZATION_F_SIMPLE4(type, name, cond, defaultval) \
	if(cond) buffer->write(#name, (type)name);

#define SERIALIZATION_F_ARRAY3(type, name, size) \
	buffer->write(#name, (type*)name, size);
#define SERIALIZATION_F_ARRAY4(type, name, size, cond) \
	if(cond) buffer->write(#name, (type*)name, size);
#define SERIALIZATION_F_ARRAY5(type, name, size, cond, defaultval) \
	if(cond) buffer->write(#name, (type*)name, size);

#define SERIALIZATION_F_DYNARRAY2(type, name) \
	buffer->write(#name, name);
#define SERIALIZATION_F_DYNARRAY3(type, name, cond) \
	if(cond) buffer->write(#name, name);
#define SERIALIZATION_F_DYNARRAY4(type, name, cond, defaultval) \
	if(cond) buffer->write(#name, name);

#define SERIALIZATION_F_COUNT3(type, name, ref) \
	buffer->write(#name, (type)ref.size());
#define SERIALIZATION_F_COUNT4(type, name, ref, cond) \
	if(cond) buffer->write(#name, (type)ref.size());
#define SERIALIZATION_F_COUNT5(type, name, ref, cond, defaultval) \
	if(cond) buffer->write(#name, (type)ref.size());

// Deserialization function
#define DESERIALIZATION_F_SIMPLE(...) OVERLOADED_CALL(DESERIALIZATION_F_SIMPLE, __VA_ARGS__)
#define DESERIALIZATION_F_ARRAY(...) OVERLOADED_CALL(DESERIALIZATION_F_ARRAY, __VA_ARGS__)
#define DESERIALIZATION_F_DYNARRAY(...) OVERLOADED_CALL(DESERIALIZATION_F_DYNARRAY, __VA_ARGS__)
#define DESERIALIZATION_F_COUNT(...) OVERLOADED_CALL(DESERIALIZATION_F_COUNT, __VA_ARGS__)

#define DESERIALIZATION_F_SIMPLE2(type, name) \
	buffer->read<type>(#name, name);
#define DESERIALIZATION_F_SIMPLE3(type, name, cond) \
	if(cond) buffer->read<type>(#name, name);
#define DESERIALIZATION_F_SIMPLE4(type, name, cond, defaultval) \
	if(cond) buffer->read<type>(#name, name); else PacketDeclaration::copyDefaultValue(name, defaultval);

#define DESERIALIZATION_F_ARRAY3(type, name, size) \
	buffer->read<type>(#name, name, size);
#define DESERIALIZATION_F_ARRAY4(type, name, size, cond) \
	if(cond) buffer->read<type>(#name, name, size);
#define DESERIALIZATION_F_ARRAY5(type, name, size, cond, defaultval) \
	if(cond) buffer->read<type>(#name, name, size); else PacketDeclaration::copyDefaultValue(name, defaultval);

#define DESERIALIZATION_F_DYNARRAY2(type, name) \
	buffer->read<type>(#name, name);
#define DESERIALIZATION_F_DYNARRAY3(type, name, cond) \
	if(cond) buffer->read<type>(#name, name);
#define DESERIALIZATION_F_DYNARRAY4(type, name, cond, defaultval) \
	if(cond) buffer->read<type>(#name, name); else PacketDeclaration::copyDefaultValue(name, defaultval);

#define DESERIALIZATION_F_COUNT3(type, name, ref) \
	ref.resize(buffer->template read<type>(#name));
#define DESERIALIZATION_F_COUNT4(type, name, ref, cond) \
	if(cond) ref.resize(buffer->template read<type>(#name));
#define DESERIALIZATION_F_COUNT5(type, name, ref, cond, defaultval) \
	if(cond) ref.resize(buffer->template read<type>(#name)); else PacketDeclaration::copyDefaultValue(name, defaultval);

// def / impl mode implementation
// needed to bypass recursion limitation
#define DEFINITION_F_SIMPLE_(...) OVERLOADED_CALL(DEFINITION_F_SIMPLE, __VA_ARGS__)
#define DEFINITION_F_ARRAY_(...) OVERLOADED_CALL(DEFINITION_F_ARRAY, __VA_ARGS__)
#define DEFINITION_F_DYNARRAY_(...) OVERLOADED_CALL(DEFINITION_F_DYNARRAY, __VA_ARGS__)
#define DEFINITION_F_COUNT_(...) OVERLOADED_CALL(DEFINITION_F_COUNT, __VA_ARGS__)
#define SERIALIZATION_F_SIMPLE_(...) OVERLOADED_CALL(SERIALIZATION_F_SIMPLE, __VA_ARGS__)
#define SERIALIZATION_F_ARRAY_(...) OVERLOADED_CALL(SERIALIZATION_F_ARRAY, __VA_ARGS__)
#define SERIALIZATION_F_DYNARRAY_(...) OVERLOADED_CALL(SERIALIZATION_F_DYNARRAY, __VA_ARGS__)
#define SERIALIZATION_F_COUNT_(...) OVERLOADED_CALL(SERIALIZATION_F_COUNT, __VA_ARGS__)
#define DESERIALIZATION_F_SIMPLE_(...) OVERLOADED_CALL(DESERIALIZATION_F_SIMPLE, __VA_ARGS__)
#define DESERIALIZATION_F_ARRAY_(...) OVERLOADED_CALL(DESERIALIZATION_F_ARRAY, __VA_ARGS__)
#define DESERIALIZATION_F_DYNARRAY_(...) OVERLOADED_CALL(DESERIALIZATION_F_DYNARRAY, __VA_ARGS__)
#define DESERIALIZATION_F_COUNT_(...) OVERLOADED_CALL(DESERIALIZATION_F_COUNT, __VA_ARGS__)
#define SIZE_F_SIMPLE_(...) OVERLOADED_CALL(SIZE_F_SIMPLE, __VA_ARGS__)
#define SIZE_F_ARRAY_(...) OVERLOADED_CALL(SIZE_F_ARRAY, __VA_ARGS__)
#define SIZE_F_DYNARRAY_(...) OVERLOADED_CALL(SIZE_F_DYNARRAY, __VA_ARGS__)
#define SIZE_F_COUNT_(...) OVERLOADED_CALL(SIZE_F_COUNT, __VA_ARGS__)

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
//#define DEFINITION_F_DYNARRAY1(mode) DEFINITION_ ## mode (DEFINITION_F_DYNARRAY_)
#define DEFINITION_F_COUNT1(mode) DEFINITION_ ## mode (DEFINITION_F_COUNT_)

#define SIZE_F_SIMPLE1(mode) SIZE_ ## mode (SIZE_F_SIMPLE_)
#define SIZE_F_ARRAY1(mode) SIZE_ ## mode (SIZE_F_ARRAY_)
//#define SIZE_F_DYNARRAY1(mode) SIZE_ ## mode (SIZE_F_DYNARRAY_)
#define SIZE_F_COUNT1(mode) SIZE_ ## mode (SIZE_F_COUNT_)

#define SERIALIZATION_F_SIMPLE1(mode) SERIALIZATION_ ## mode (SERIALIZATION_F_SIMPLE_)
#define SERIALIZATION_F_ARRAY1(mode) SERIALIZATION_ ## mode (SERIALIZATION_F_ARRAY_)
//#define SERIALIZATION_F_DYNARRAY1(mode) SERIALIZATION_ ## mode (SERIALIZATION_F_DYNARRAY_)
#define SERIALIZATION_F_COUNT1(mode) SERIALIZATION_ ## mode (SERIALIZATION_F_COUNT_)

#define DESERIALIZATION_F_SIMPLE1(mode) DESERIALIZATION_ ## mode (DESERIALIZATION_F_SIMPLE_)
#define DESERIALIZATION_F_ARRAY1(mode) DESERIALIZATION_ ## mode (DESERIALIZATION_F_ARRAY_)
//#define DESERIALIZATION_F_DYNARRAY1(mode) DESERIALIZATION_ ## mode (DESERIALIZATION_F_DYNARRAY_)
#define DESERIALIZATION_F_COUNT1(mode) DESERIALIZATION_ ## mode (DESERIALIZATION_F_COUNT_)

#define CREATE_STRUCT(name) \
	struct name { \
		name ## _DEF(DEFINITION_F_SIMPLE, DEFINITION_F_ARRAY, DEFINITION_F_DYNARRAY, DEFINITION_F_COUNT) \
	\
		uint32_t getSize(int version) const { \
			uint32_t size = 0; \
			name ## _DEF(SIZE_F_SIMPLE, SIZE_F_ARRAY, SIZE_F_DYNARRAY, SIZE_F_COUNT) \
	\
			return size;\
		} \
	\
		template<class T> \
		void serialize(T* buffer) const { \
			const int version = buffer->version; \
			name ## _DEF(SERIALIZATION_F_SIMPLE, SERIALIZATION_F_ARRAY, SERIALIZATION_F_DYNARRAY, SERIALIZATION_F_COUNT) \
		} \
	\
		template<class T> \
		void deserialize(T* buffer) { \
			const int version = buffer->version; \
	\
			name ## _DEF(DESERIALIZATION_F_SIMPLE, DESERIALIZATION_F_ARRAY, DESERIALIZATION_F_DYNARRAY, DESERIALIZATION_F_COUNT) \
		} \
	}

#define CREATE_PACKET(name, id) \
	struct name : public TS_MESSAGE_BASE { \
		static const uint16_t packetID = id; \
		name() : TS_MESSAGE_BASE(packetID) {} \
	\
		name ## _DEF(DEFINITION_F_SIMPLE, DEFINITION_F_ARRAY, DEFINITION_F_DYNARRAY, DEFINITION_F_COUNT) \
	\
		uint32_t getSize(int version) const { \
			uint32_t size = TS_MESSAGE_BASE::getSize(version); \
			name ## _DEF(SIZE_F_SIMPLE, SIZE_F_ARRAY, SIZE_F_DYNARRAY, SIZE_F_COUNT) \
	\
			return size;\
		} \
	\
		template<class T> \
		void serialize(T* buffer) const { \
			const int version = buffer->version; \
			TS_MESSAGE_BASE::serialize(buffer); \
	\
			name ## _DEF(SERIALIZATION_F_SIMPLE, SERIALIZATION_F_ARRAY, SERIALIZATION_F_DYNARRAY, SERIALIZATION_F_COUNT) \
		} \
	\
		template<class T> \
		void deserialize(T* buffer) { \
			const int version = buffer->version; \
			TS_MESSAGE_BASE::deserialize(buffer); \
	\
			name ## _DEF(DESERIALIZATION_F_SIMPLE, DESERIALIZATION_F_ARRAY, DESERIALIZATION_F_DYNARRAY, DESERIALIZATION_F_COUNT) \
		} \
	}


#ifdef TEST_PACKET
struct TS_MESSAGE_BASE {
	TS_MESSAGE_BASE(int id) {}
	template<class T>
	void serialize(T* buffer) const {}
	template<class T>
	void deserialize(T* buffer) {}
};
#endif

#endif // PACKETDECLARATION_H

