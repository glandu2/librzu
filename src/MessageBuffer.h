#ifndef MESSAGEBUFFER_H
#define MESSAGEBUFFER_H

#include "Socket.h"
#include <vector>
#include <string.h>
#include <type_traits>

class MessageBuffer {
public:
	Stream::WriteRequest* buffer;
	char *p;
	int version;

	MessageBuffer(size_t size) {
		buffer = Stream::WriteRequest::create(size);
		p = buffer->buffer.base;
	}

	MessageBuffer(const char* data, size_t size) {
		buffer = Stream::WriteRequest::create(size);
		p = buffer->buffer.base;
		memcpy(buffer->buffer.base, data, size);
	}

	const char* getData() const { return buffer->buffer.base; }
	char* getData() { return buffer->buffer.base; }
	uint32_t getSize() const { return buffer->buffer.len; }
	uint16_t getMessageId() const  { return *reinterpret_cast<const uint16_t*>(buffer->buffer.base + 4); }

	void reset() {
		p = buffer->buffer.base;
	}

	bool checkFinalSize() const {
		uint32_t msgSize = *reinterpret_cast<const uint32_t*>(buffer->buffer.base);
		return uint32_t(p - buffer->buffer.base) == msgSize;
	}

	// Write functions /////////////////////////

	//Primitives
	template<typename T>
	typename std::enable_if<std::is_fundamental<T>::value, void>::type
	write(const char*, T val) {
		*reinterpret_cast<T*>(p) = val;
		p += sizeof(T);
	}

	//Objects
	template<typename T>
	typename std::enable_if<!std::is_fundamental<T>::value, void>::type
	write(const char*, const T& val) {
		val.serialize(this);
	}

	//Fixed array of primitive
	template<typename T>
	typename std::enable_if<std::is_fundamental<T>::value, void>::type
	write(const char*, const T* val, int size) {
		memcpy(p, val, sizeof(T) * size);
		p += sizeof(T) * size;
	}

	//Fixed array of object
	template<typename T>
	typename std::enable_if<!std::is_fundamental<T>::value, void>::type
	write(const char* fieldName, const T* val, int size) {
		for(size_t i = 0; i < size; i++) {
			write(fieldName, val[i]);
		}
	}

	//String
	void write(const char*, const std::string& val, int size) {
		size_t strSize = val.size() > size ? size : val.size();
		memcpy(p, val.c_str(), strSize);
		memset(p + strSize, 0, size - strSize);
		p += size;
	}

	//Dynamic array of object
	template<class U>
	void write(const char* fieldName, const std::vector<U>& val) {
		auto it = val.begin();
		auto itEnd = val.end();
		for(; it != itEnd; ++it)
			write(fieldName, *it);
	}

	// Read functions /////////////////////////

	//Primitives via arg
	template<typename T>
	typename std::enable_if<std::is_fundamental<T>::value, void>::type
	read(const char*, T& val) {
		val = *reinterpret_cast<T*>(p);
		p += sizeof(T);
	}

	//Primitives via return type (for vector.resize())
	template<typename T>
	typename std::enable_if<std::is_fundamental<T>::value, T>::type
	read(const char*) {
		T val = *reinterpret_cast<T*>(p);
		p += sizeof(T);
		return val;
	}

	//Objects
	template<typename T>
	typename std::enable_if<!std::is_fundamental<T>::value, void>::type
	read(const char*, T& val) {
		val.deserialize(this);
	}

	//Fixed array of primitive
	template<typename T>
	typename std::enable_if<std::is_fundamental<T>::value, void>::type
	read(const char*, T* val, int size) {
		memcpy(val, p, sizeof(T) * size);
		p += sizeof(T) * size;
	}

	//Fixed array of objects
	template<typename T>
	typename std::enable_if<!std::is_fundamental<T>::value, void>::type
	read(const char* fieldName, T* val, int size) {
		for(size_t i = 0; i < size; i++) {
			read(fieldName, val[i]);
		}
	}

	//String
	void read(const char*, std::string& val, int size) {
		val.assign(p);
		p += size;
	}

	//Dynamic array of object
	template<class U>
	void read(const char* fieldName, std::vector<U>& val) {
		auto it = val.begin();
		auto itEnd = val.end();
		for(; it != itEnd; ++it)
			read(fieldName, *it);
	}

	//Dummy read
	void discard(const char*, int size) {
		p += size;
	}
};

#endif // MESSAGEBUFFER_H
