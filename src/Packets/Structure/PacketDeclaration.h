#ifndef PACKETDECLARATION_H
#define PACKETDECLARATION_H

#include <string.h>
#include <stdint.h>
#include <vector>
#include "../PacketBaseMessage.h"

#define _ARG5(_0, _1, _2, _3, _4, _5, ...) _5
#define _EXPAND(x) x
#define NARG5(...) _EXPAND(_ARG5(__VA_ARGS__, 5, 4, 3, 2, 1, 0))

#define __ONE_OR_TWO_ARGS(MACRO, N, ...) MACRO ## N (__VA_ARGS__)
#define _ONE_OR_TWO_ARGS(MACRO, N, ...) __ONE_OR_TWO_ARGS(MACRO, N, __VA_ARGS__)

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

} // namespace PacketDeclaration


// Body fields
#define DEFINITION_F_SIMPLE(...) _ONE_OR_TWO_ARGS(DEFINITION_F_SIMPLE, NARG5(__VA_ARGS__), __VA_ARGS__)
#define DEFINITION_F_ARRAY(...) _ONE_OR_TWO_ARGS(DEFINITION_F_ARRAY, NARG5(__VA_ARGS__), __VA_ARGS__)
#define DEFINITION_F_DYNARRAY(...) _ONE_OR_TWO_ARGS(DEFINITION_F_DYNARRAY, NARG5(__VA_ARGS__), __VA_ARGS__)
#define DEFINITION_F_COUNT(...) _ONE_OR_TWO_ARGS(DEFINITION_F_COUNT, NARG5(__VA_ARGS__), __VA_ARGS__)

#define DEFINITION_F_SIMPLE2(type, name) type name;
#define DEFINITION_F_SIMPLE3(type, name, cond) type name;
#define DEFINITION_F_SIMPLE4(type, name, cond, defaultval) type name;

#define DEFINITION_F_ARRAY3(type, name, size) type name[size];
#define DEFINITION_F_ARRAY4(type, name, size, cond) type name[size];
#define DEFINITION_F_ARRAY5(type, name, size, cond, defaultval) type name[size];

#define DEFINITION_F_DYNARRAY2(type, name) std::vector<type> name;
#define DEFINITION_F_DYNARRAY3(type, name, cond) std::vector<type> name;
#define DEFINITION_F_DYNARRAY4(type, name, cond, defaultval) std::vector<type> name;

#define DEFINITION_F_COUNT3(type, name, ref) type name;
#define DEFINITION_F_COUNT4(type, name, ref, cond) type name;
#define DEFINITION_F_COUNT5(type, name, ref, cond, defaultval) type name;

// Serialization function
#define SERIALIZATION_F_SIMPLE(...) _ONE_OR_TWO_ARGS(SERIALIZATION_F_SIMPLE, NARG5(__VA_ARGS__), __VA_ARGS__)
#define SERIALIZATION_F_ARRAY(...) _ONE_OR_TWO_ARGS(SERIALIZATION_F_ARRAY, NARG5(__VA_ARGS__), __VA_ARGS__)
#define SERIALIZATION_F_DYNARRAY(...) _ONE_OR_TWO_ARGS(SERIALIZATION_F_DYNARRAY, NARG5(__VA_ARGS__), __VA_ARGS__)
#define SERIALIZATION_F_COUNT(...) _ONE_OR_TWO_ARGS(SERIALIZATION_F_COUNT, NARG5(__VA_ARGS__), __VA_ARGS__)

#define SERIALIZATION_F_SIMPLE2(type, name) \
	buffer->write(#name, name);
#define SERIALIZATION_F_SIMPLE3(type, name, cond) \
	if(cond) buffer->write(#name, name);
#define SERIALIZATION_F_SIMPLE4(type, name, cond, defaultval) \
	if(cond) buffer->write(#name, name);

#define SERIALIZATION_F_ARRAY3(type, name, size) \
	buffer->write(#name, name, size);
#define SERIALIZATION_F_ARRAY4(type, name, size, cond) \
	if(cond) buffer->write(#name, name, size);
#define SERIALIZATION_F_ARRAY5(type, name, size, cond, defaultval) \
	if(cond) buffer->write(#name, name, size);

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
#define DESERIALIZATION_F_SIMPLE(...) _ONE_OR_TWO_ARGS(DESERIALIZATION_F_SIMPLE, NARG5(__VA_ARGS__), __VA_ARGS__)
#define DESERIALIZATION_F_ARRAY(...) _ONE_OR_TWO_ARGS(DESERIALIZATION_F_ARRAY, NARG5(__VA_ARGS__), __VA_ARGS__)
#define DESERIALIZATION_F_DYNARRAY(...) _ONE_OR_TWO_ARGS(DESERIALIZATION_F_DYNARRAY, NARG5(__VA_ARGS__), __VA_ARGS__)
#define DESERIALIZATION_F_COUNT(...) _ONE_OR_TWO_ARGS(DESERIALIZATION_F_COUNT, NARG5(__VA_ARGS__), __VA_ARGS__)

#define DESERIALIZATION_F_SIMPLE2(type, name) \
	buffer->read(#name, name);
#define DESERIALIZATION_F_SIMPLE3(type, name, cond) \
	if(cond) buffer->read(#name, name);
#define DESERIALIZATION_F_SIMPLE4(type, name, cond, defaultval) \
	if(cond) buffer->read(#name, name); else PacketDeclaration::copyDefaultValue(name, defaultval);

#define DESERIALIZATION_F_ARRAY3(type, name, size) \
	buffer->read(#name, name, size);
#define DESERIALIZATION_F_ARRAY4(type, name, size, cond) \
	if(cond) buffer->read(#name, name, size);
#define DESERIALIZATION_F_ARRAY5(type, name, size, cond, defaultval) \
	if(cond) buffer->read(#name, name, size); else PacketDeclaration::copyDefaultValue(name, defaultval);

#define DESERIALIZATION_F_DYNARRAY2(type, name) \
	buffer->read(#name, name);
#define DESERIALIZATION_F_DYNARRAY3(type, name, cond) \
	if(cond) buffer->read(#name, name);
#define DESERIALIZATION_F_DYNARRAY4(type, name, cond, defaultval) \
	if(cond) buffer->read(#name, name); else PacketDeclaration::copyDefaultValue(name, defaultval);

#define DESERIALIZATION_F_COUNT3(type, name, ref) \
	ref.resize(buffer->template read<type>(#name));
#define DESERIALIZATION_F_COUNT4(type, name, ref, cond) \
	if(cond) ref.resize(buffer->template read<type>(#name));
#define DESERIALIZATION_F_COUNT5(type, name, ref, cond, defaultval) \
	if(cond) ref.resize(buffer->template read<type>(#name)); else PacketDeclaration::copyDefaultValue(name, defaultval);

#define CREATE_STRUCT(name, body) \
	struct name { \
		body(DEFINITION_F_SIMPLE, DEFINITION_F_ARRAY, DEFINITION_F_DYNARRAY, DEFINITION_F_COUNT) \
	\
		template<class T> \
		void serialize(T* buffer) const { \
			const int version = buffer->version; \
			body(SERIALIZATION_F_SIMPLE, SERIALIZATION_F_ARRAY, SERIALIZATION_F_DYNARRAY, SERIALIZATION_F_COUNT) \
		} \
	\
		template<class T> \
		void deserialize(T* buffer) { \
			const int version = buffer->version; \
	\
			body(DESERIALIZATION_F_SIMPLE, DESERIALIZATION_F_ARRAY, DESERIALIZATION_F_DYNARRAY, DESERIALIZATION_F_COUNT) \
		} \
	}
/*
struct TS_MESSAGE_BASE {
	TS_MESSAGE_BASE(int id) {}
	template<class T>
	void serialize(T* buffer) const {}
	template<class T>
	void deserialize(T* buffer) {}
};*/

#define CREATE_PACKET(name, body, id) \
	struct name : public TS_MESSAGE_BASE { \
		static const uint16_t packetID = id; \
		name() : TS_MESSAGE_BASE(packetID) {} \
	\
		body(DEFINITION_F_SIMPLE, DEFINITION_F_ARRAY, DEFINITION_F_DYNARRAY, DEFINITION_F_COUNT) \
	\
		template<class T> \
		void serialize(T* buffer) const { \
			const int version = buffer->version; \
			TS_MESSAGE_BASE::serialize(buffer); \
	\
			body(SERIALIZATION_F_SIMPLE, SERIALIZATION_F_ARRAY, SERIALIZATION_F_DYNARRAY, SERIALIZATION_F_COUNT) \
		} \
	\
		template<class T> \
		void deserialize(T* buffer) { \
			const int version = buffer->version; \
			TS_MESSAGE_BASE::deserialize(buffer); \
	\
			body(DESERIALIZATION_F_SIMPLE, DESERIALIZATION_F_ARRAY, DESERIALIZATION_F_DYNARRAY, DESERIALIZATION_F_COUNT) \
		} \
	}

#endif // PACKETDECLARATION_H

