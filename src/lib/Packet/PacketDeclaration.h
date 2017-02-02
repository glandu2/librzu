#ifndef PACKETDECLARATION_H
#define PACKETDECLARATION_H

#ifndef DEBUG_PREPROCESSOR
#include <string.h>
#include <stdint.h>
#include <vector>
#include <string>
#include "PacketEpics.h"
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

#ifndef DEBUG_PREPROCESSOR
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
#endif

// Dispatch declarations
#define DEFINITION_F(x) DEFINITION_F_##x
#define SIZE_F(x) SIZE_F_##x
#define SERIALIZATION_F(x) SERIALIZATION_F_##x
#define DESERIALIZATION_F(x) DESERIALIZATION_F_##x

// Body fields
#define DEFINITION_F_simple(type, name, ...) type name;
#define DEFINITION_F_array(type, name, size, ...) type name[size];
#define DEFINITION_F_dynarray(type, name, ...) std::vector<type> name; struct _metadata_##name { enum { addNullTerminator = 0 }; };
#define DEFINITION_F_count(type, ref, ...)
#define DEFINITION_F_string(name, size, ...) std::string name;
#define DEFINITION_F_dynstring(name, hasNullTerminator, ...) std::string name; struct _metadata_##name { enum { addNullTerminator = !!hasNullTerminator }; };
#define DEFINITION_F_padmarker(...)
#define DEFINITION_F_pad(...)
#define DEFINITION_F_autocount(ref, ...)

// Size function
#define SIZE_F_simple(...) OVERLOADED_CALL(SIZE_F_SIMPLE, __VA_ARGS__)
#define SIZE_F_array(...) OVERLOADED_CALL(SIZE_F_ARRAY, __VA_ARGS__)
#define SIZE_F_dynarray(...) OVERLOADED_CALL(SIZE_F_DYNARRAY, __VA_ARGS__)
#define SIZE_F_count(...) OVERLOADED_CALL(SIZE_F_COUNT, __VA_ARGS__)
#define SIZE_F_string(...) OVERLOADED_CALL(SIZE_F_STRING, __VA_ARGS__)
#define SIZE_F_dynstring(...) OVERLOADED_CALL(SIZE_F_DYNSTRING, __VA_ARGS__)
#define SIZE_F_padmarker(...) OVERLOADED_CALL(SIZE_F_PADMARKER, __VA_ARGS__)
#define SIZE_F_pad(...) OVERLOADED_CALL(SIZE_F_PAD, __VA_ARGS__)
#define SIZE_F_autocount(...)

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

#define SIZE_F_COUNT2(type, ref) \
	size += sizeof(type);

#define SIZE_F_STRING2(name, _size) \
	size += _size;
#define SIZE_F_STRING3(name, _size, cond) \
	if(cond) size += _size;
#define SIZE_F_STRING4(name, _size, cond, defaultval) \
	if(cond) size += _size;

#define SIZE_F_DYNSTRING2(name, hasNullTerminator) \
	size += (uint32_t)name.size() + !!hasNullTerminator;
#define SIZE_F_DYNSTRING3(name, hasNullTerminator, cond) \
	if(cond) size += (uint32_t)name.size() + !!hasNullTerminator;
#define SIZE_F_DYNSTRING4(name, hasNullTerminator, cond, defaultval) \
	if(cond) size += (uint32_t)name.size() + !!hasNullTerminator;

#define SIZE_F_PADMARKER1(marker) \
	const uint32_t marker = size;
#define SIZE_F_PAD2(_size, marker) \
	if(size < marker + (_size)) size = marker + (_size);
#define SIZE_F_PAD3(_size, marker, cond) \
	if(cond && size < marker + (_size)) size = marker + (_size);

// Serialization function
#define SERIALIZATION_F_simple(...) OVERLOADED_CALL(SERIALIZATION_F_SIMPLE, __VA_ARGS__)
#define SERIALIZATION_F_array(...) OVERLOADED_CALL(SERIALIZATION_F_ARRAY, __VA_ARGS__)
#define SERIALIZATION_F_dynarray(...) OVERLOADED_CALL(SERIALIZATION_F_DYNARRAY, __VA_ARGS__)
#define SERIALIZATION_F_count(...) OVERLOADED_CALL(SERIALIZATION_F_COUNT, __VA_ARGS__)
#define SERIALIZATION_F_string(...) OVERLOADED_CALL(SERIALIZATION_F_STRING, __VA_ARGS__)
#define SERIALIZATION_F_dynstring(...) OVERLOADED_CALL(SERIALIZATION_F_DYNSTRING, __VA_ARGS__)
#define SERIALIZATION_F_padmarker(...) OVERLOADED_CALL(SERIALIZATION_F_PADMARKER, __VA_ARGS__)
#define SERIALIZATION_F_pad(...) OVERLOADED_CALL(SERIALIZATION_F_PAD, __VA_ARGS__)
#define SERIALIZATION_F_autocount(...) OVERLOADED_CALL(SERIALIZATION_F_AUTOCOUNT, __VA_ARGS__)

#define SERIALIZATION_F_SIMPLE2(type, name) \
	buffer->write(#name, (type)name);
#define SERIALIZATION_F_SIMPLE3(type, name, cond) \
	if(cond) buffer->write(#name, (type)name);
#define SERIALIZATION_F_SIMPLE4(type, name, cond, defaultval) \
	if(cond) buffer->write(#name, (type)name);

#define SERIALIZATION_F_ARRAY3(type, name, size) \
	buffer->template writeArray<type>(#name, name, size);
#define SERIALIZATION_F_ARRAY4(type, name, size, cond) \
	if(cond) buffer->template writeArray<type>(#name, name, size);
#define SERIALIZATION_F_ARRAY5(type, name, size, cond, defaultval) \
	if(cond) buffer->template writeArray<type>(#name, name, size);

#define SERIALIZATION_F_DYNARRAY2(type, name) \
	buffer->template writeDynArray<type>(#name, name);
#define SERIALIZATION_F_DYNARRAY3(type, name, cond) \
	if(cond) buffer->template writeDynArray<type>(#name, name);
#define SERIALIZATION_F_DYNARRAY4(type, name, cond, defaultval) \
	if(cond) buffer->template writeDynArray<type>(#name, name);

#define SERIALIZATION_F_COUNT2(type, ref) \
	buffer->write(#ref "_size", (type)(ref.size() + _metadata_##ref::addNullTerminator));

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

#define SERIALIZATION_F_PADMARKER1(marker) \
	const uint32_t marker = buffer->getParsedSize();
#define SERIALIZATION_F_PAD2(_size, marker) \
	if(buffer->getParsedSize() < marker + (_size)) \
	    buffer->pad("pad_" #marker, marker + (_size) - buffer->getParsedSize());
#define SERIALIZATION_F_PAD3(_size, marker, cond) \
	if(cond && buffer->getParsedSize() < marker + (_size)) \
	    buffer->pad("pad_" #marker, marker + (_size) - buffer->getParsedSize());

#define SERIALIZATION_F_AUTOCOUNT1(ref)

// Deserialization function
#define DESERIALIZATION_F_simple(...) OVERLOADED_CALL(DESERIALIZATION_F_SIMPLE, __VA_ARGS__)
#define DESERIALIZATION_F_array(...) OVERLOADED_CALL(DESERIALIZATION_F_ARRAY, __VA_ARGS__)
#define DESERIALIZATION_F_dynarray(...) OVERLOADED_CALL(DESERIALIZATION_F_DYNARRAY, __VA_ARGS__)
#define DESERIALIZATION_F_count(...) OVERLOADED_CALL(DESERIALIZATION_F_COUNT, __VA_ARGS__)
#define DESERIALIZATION_F_string(...) OVERLOADED_CALL(DESERIALIZATION_F_STRING, __VA_ARGS__)
#define DESERIALIZATION_F_dynstring(...) OVERLOADED_CALL(DESERIALIZATION_F_DYNSTRING, __VA_ARGS__)
#define DESERIALIZATION_F_pad(...) OVERLOADED_CALL(DESERIALIZATION_F_PAD, __VA_ARGS__)
#define DESERIALIZATION_F_padmarker(...) OVERLOADED_CALL(DESERIALIZATION_F_PADMARKER, __VA_ARGS__)
#define DESERIALIZATION_F_autocount(...) OVERLOADED_CALL(DESERIALIZATION_F_AUTOCOUNT, __VA_ARGS__)

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
	if(cond) buffer->template readArray<type>(#name, name, size); \
	else { \
		static const type defaultArray[size] = defaultval; \
		PacketDeclaration::copyDefaultValue(name, defaultArray, size); \
	}

#define DESERIALIZATION_F_DYNARRAY2(type, name) \
	buffer->template readDynArray<type>(#name, name, name##_size);
#define DESERIALIZATION_F_DYNARRAY3(type, name, cond) \
	if(cond) buffer->template readDynArray<type>(#name, name, name##_size);
#define DESERIALIZATION_F_DYNARRAY4(type, name, cond, defaultval) \
	if(cond) buffer->template readDynArray<type>(#name, name, name##_size); else PacketDeclaration::copyDefaultValue(name, defaultval);

#define DESERIALIZATION_F_COUNT2(type, ref) \
	uint32_t ref##_size; \
	buffer->template read<type>(#ref "_size", ref##_size);

#define DESERIALIZATION_F_STRING2(name, size) \
	buffer->readString(#name, name, size);
#define DESERIALIZATION_F_STRING3(name, size, cond) \
	if(cond) buffer->readString(#name, name, size);
#define DESERIALIZATION_F_STRING4(name, size, cond, defaultval) \
	if(cond) buffer->readString(#name, name, size); \
	else name = defaultval;

#define DESERIALIZATION_F_DYNSTRING2(name, hasNullTerminator) \
	buffer->readDynString(#name, name, name##_size, hasNullTerminator);
#define DESERIALIZATION_F_DYNSTRING3(name, hasNullTerminator, cond) \
	if(cond) buffer->readDynString(#name, name, name##_size, hasNullTerminator);
#define DESERIALIZATION_F_DYNSTRING4(name, hasNullTerminator, cond, defaultval) \
	if(cond) buffer->readDynString(#name, name, name##_size, hasNullTerminator); \
	else name = defaultval;


#define DESERIALIZATION_F_PADMARKER1(marker) \
	const uint32_t marker = buffer->getParsedSize();
#define DESERIALIZATION_F_PAD2(_size, marker) \
	if(buffer->getParsedSize() < marker + (_size)) \
	    buffer->discard("pad_" #marker, marker + (_size) - buffer->getParsedSize());
#define DESERIALIZATION_F_PAD3(_size, marker, cond) \
	if(cond && buffer->getParsedSize() < marker + (_size)) \
	    buffer->discard("pad_" #marker, marker + (_size) - buffer->getParsedSize());

#define DESERIALIZATION_F_AUTOCOUNT1(ref) \
	uint32_t ref##_size; \
	buffer->readRemainingSize(#ref "_size", ref##_size);

// def / impl mode implementation
#define DO_NOTHING(...)

#define DEFINITION_F_def(x) DEFINITION_F_##x
#define SIZE_F_def(x) DO_NOTHING
#define SERIALIZATION_F_def(x) DO_NOTHING
#define DESERIALIZATION_F_def(x) DO_NOTHING

#define DEFINITION_F_impl(x) DO_NOTHING
#define SIZE_F_impl(x) SIZE_F_##x
#define SERIALIZATION_F_impl(x) SERIALIZATION_F_##x
#define DESERIALIZATION_F_impl(x) DESERIALIZATION_F_##x

#define CREATE_STRUCT_IMPL(name_, size_base_, definition_header_, serialization_header_, deserialization_header_) \
	struct name_ { \
		definition_header_ \
		name_ ## _DEF(DEFINITION_F) \
	\
		uint32_t getSize(int version) const { \
			uint32_t size = size_base_; \
			(void)(version); \
			name_ ## _DEF(SIZE_F) \
	\
			return size;\
		} \
	\
		template<class T> \
		void serialize(T* buffer) const { \
			const int version = buffer->getVersion(); \
			(void)(version); \
			serialization_header_ \
			name_ ## _DEF(SERIALIZATION_F) \
		} \
	\
		template<class T> \
		void deserialize(T* buffer) { \
			const int version = buffer->getVersion(); \
			(void)(version); \
			deserialization_header_ \
	\
			name_ ## _DEF(DESERIALIZATION_F) \
	    } \
	}

#define CREATE_STRUCT(name_) \
	CREATE_STRUCT_IMPL(name_, 0, /* empty */, /* empty */, /* empty */)


#define CREATE_PACKET_DEFINITION_HEADER(id_) \
	static const uint16_t packetID = id_; \
	uint16_t id;

#define CREATE_PACKET_SERIALIZATION_HEADER \
	uint32_t size = getSize(buffer->getVersion()); \
	buffer->template write<uint32_t>("size", size); \
	buffer->template write<uint16_t>("id", packetID); \
	buffer->template write<uint8_t>("msg_checksum", getMessageChecksum(size, packetID));

#define CREATE_PACKET_DESERIALIZATION_HEADER \
	buffer->discard("size", 4); \
	buffer->template read<uint16_t>("id", id); \
	buffer->discard("msg_checksum", 1);

#define CREATE_PACKET(name_, id_) \
	CREATE_STRUCT_IMPL(name_, 7, CREATE_PACKET_DEFINITION_HEADER(id_), CREATE_PACKET_SERIALIZATION_HEADER, CREATE_PACKET_DESERIALIZATION_HEADER)


#define HEADER_F_ID(id_, ...) \
	static const uint16_t packetID_##id_ = id_;

#define SERIALISATION_F_ID(...) OVERLOADED_CALL(SERIALISATION_F_ID, __VA_ARGS__)
#define SERIALISATION_F_ID1(id_) \
	id = id_;
#define SERIALISATION_F_ID2(id_, condition_) \
	if(condition_) id = id_;

#define CREATE_PACKET_VER_ID_HEADER_HEADER(name_) \
	name_ ## _ID(HEADER_F_ID) \
	uint16_t id;

#define CREATE_PACKET_VER_ID_SERIALIZATION_HEADER(name_) \
	uint32_t size = getSize(buffer->getVersion()); \
	uint16_t id; \
	name_ ## _ID(SERIALISATION_F_ID) \
	buffer->template write<uint32_t>("size", size); \
	buffer->template write<uint16_t>("id", id); \
	buffer->template write<uint8_t>("msg_checksum", getMessageChecksum(size, id));

#define CREATE_PACKET_VER_ID_DESERIALIZATION_HEADER \
	buffer->discard("size", 4); \
	buffer->template read<uint16_t>("id", id); \
	buffer->discard("msg_checksum", 1);

#define CREATE_PACKET_VER_ID(name_) \
	CREATE_STRUCT_IMPL(name_, 7, CREATE_PACKET_VER_ID_HEADER_HEADER(name_), CREATE_PACKET_VER_ID_SERIALIZATION_HEADER(name_), CREATE_PACKET_VER_ID_DESERIALIZATION_HEADER)

#define CREATE_PACKET_VER_ID_SWITCH_CASES_2(id_, ...) \
	packetID_##id_:

#define CREATE_PACKET_VER_ID_SWITCH_CASES(name_) \
	case name_:: CREATE_PACKET_VER_ID_SWITCH_CASES_2

#define case_packet_is(name_) \
	name_ ## _ID(CREATE_PACKET_VER_ID_SWITCH_CASES(name_))

#endif // PACKETDECLARATION_H

