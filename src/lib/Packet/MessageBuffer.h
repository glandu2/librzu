#ifndef MESSAGEBUFFER_H
#define MESSAGEBUFFER_H

#include "Core/Object.h"
#include "Stream/Stream.h"
#include <vector>
#include <string.h>
#include <type_traits>

class MessageBuffer : public Object {
private:
	Stream::WriteRequest* buffer;
	char *p;
	int version;
	bool bufferOverflow;
	std::string fieldInOverflow;

public:

	MessageBuffer(size_t size, int version) {
		buffer = Stream::WriteRequest::create(size);
		p = buffer->buffer.base;
		this->version = version;
		bufferOverflow = false;
	}

	MessageBuffer(const void* data, size_t size, int version) {
		buffer = Stream::WriteRequest::createFromExisting(const_cast<char*>(static_cast<const char*>(data)), size);
		p = buffer->buffer.base;
		this->version = version;
		bufferOverflow = false;
	}

	~MessageBuffer() {
		if(buffer) {
			Stream::WriteRequest::destroy(buffer);
		}
	}

	const char* getData() const { return buffer->buffer.base; }
	char* getData() { return buffer->buffer.base; }
	uint32_t getSize() const { return buffer->buffer.len; }
	uint16_t getMessageId() const  { return *reinterpret_cast<const uint16_t*>(buffer->buffer.base + 4); }
	std::string getFieldInOverflow() const  { return fieldInOverflow; }
	int getVersion() const { return version; }

	bool checkFinalSize() {
		uint32_t msgSize = *reinterpret_cast<const uint32_t*>(buffer->buffer.base);
		bool ok = !bufferOverflow && msgSize == getSize() && uint32_t(p - buffer->buffer.base) == msgSize;
		if(!ok)
			error("Packet has invalid data: id: %d, size: %d, field: %s\n", getMessageId(), getSize(), getFieldInOverflow().c_str());
		return ok;
	}

	bool checkAvailableBuffer(const char* fieldName, size_t size) {
		if(bufferOverflow == false) {
			bool ok = size >= 0 && size < 65536 && size_t(p + size - getData()) <= size_t(getSize());
			if(!ok) {
				bufferOverflow = true;
				fieldInOverflow = fieldName;
			}
			return ok;
		} else {
			return false;
		}
	}

	// Write functions /////////////////////////

	//Primitives
	template<typename T>
	typename std::enable_if<std::is_fundamental<T>::value, void>::type
	write(const char* fieldName, T val) {
		if(checkAvailableBuffer(fieldName, sizeof(T))) {
			*reinterpret_cast<T*>(p) = val;
			p += sizeof(T);
		}
	}

	//Objects
	template<typename T>
	typename std::enable_if<!std::is_fundamental<T>::value, void>::type
	write(const char* fieldName, const T& val) {
		if(checkAvailableBuffer(fieldName, 1)) {
			val.serialize(this);
		}
	}

	//Fixed array of primitive
	template<typename T>
	typename std::enable_if<std::is_fundamental<T>::value, void>::type
	writeArray(const char* fieldName, const T* val, size_t size) {
		if(checkAvailableBuffer(fieldName, sizeof(T) * size)) {
			memcpy(p, val, sizeof(T) * size);
			p += sizeof(T) * size;
		}
	}

	//Fixed array of object
	template<typename T>
	typename std::enable_if<!std::is_fundamental<T>::value, void>::type
	writeArray(const char* fieldName, const T* val, size_t size) {
		for(size_t i = 0; i < size; i++) {
			write(fieldName, val[i]);
		}
	}

	//Dynamic array of object
	template<class T, class U>
	void writeDynArray(const char* fieldName, const std::vector<U>& val) {
		auto it = val.begin();
		auto itEnd = val.end();
		for(; it != itEnd; ++it)
			write(fieldName, (T)*it);
	}

	// Read functions /////////////////////////

	//Primitives via arg
	template<typename T, typename U>
	typename std::enable_if<std::is_fundamental<U>::value, void>::type
	read(const char* fieldName, U& val) {
		if(checkAvailableBuffer(fieldName, sizeof(T))) {
			val = (U)*reinterpret_cast<T*>(p);
			p += sizeof(T);
		}
	}

	//Objects
	template<typename T>
	typename std::enable_if<!std::is_fundamental<T>::value, void>::type
	read(const char* fieldName, T& val) {
		if(checkAvailableBuffer(fieldName, 1)) {
			val.deserialize(this);
		}
	}

	//Fixed array of primitive
	template<typename T, typename U>
	typename std::enable_if<std::is_fundamental<U>::value && !std::is_same<T, U>::value, void>::type
	readArray(const char* fieldName, U* val, size_t size) {
		if(checkAvailableBuffer(fieldName, sizeof(T) * size)) {
			for(size_t i = 0; i < size; i++) {
				val[i] = (U)*reinterpret_cast<T*>(p);
				p += sizeof(T);
			}
		}
	}

	//If cast not needed
	template<typename T>
	typename std::enable_if<std::is_fundamental<T>::value, void>::type
	readArray(const char* fieldName, T* val, size_t size) {
		if(checkAvailableBuffer(fieldName, sizeof(T) * size)) {
			memcpy(val, p, sizeof(T) * size);
			p += sizeof(T) * size;
		}
	}

	//Fixed array of objects
	template<typename T>
	typename std::enable_if<!std::is_fundamental<T>::value, void>::type
	readArray(const char* fieldName, T* val, size_t size) {
		for(size_t i = 0; i < size; i++) {
			read(fieldName, val[i]);
		}
	}

	//Dynamic array of object
	template<class U>
	void readDynArray(const char* fieldName, std::vector<U>& val) {
		auto it = val.begin();
		auto itEnd = val.end();
		for(; it != itEnd; ++it)
			read(fieldName, *it);
	}

	//read size for objects
	template<typename T, class U>
	void readSize(const char* fieldName, std::vector<U>& vec) {
		if(checkAvailableBuffer(fieldName, sizeof(T))) {
			size_t val = *reinterpret_cast<T*>(p);
			p += sizeof(T);
			if(checkAvailableBuffer(fieldName, val))
				vec.resize(val);
		}
	}

	void discard(const char* fieldName, size_t size) {
		if(checkAvailableBuffer(fieldName, size)) {
			p += size;
		}
	}
};

#endif // MESSAGEBUFFER_H
