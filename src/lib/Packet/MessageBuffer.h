#ifndef MESSAGEBUFFER_H
#define MESSAGEBUFFER_H

#include "Core/Object.h"
#include "Stream/Stream.h"
#include "StructSerializer.h"
#include <string.h>
#include <type_traits>
#include <vector>

class RZU_EXTERN MessageBuffer : public StructSerializer, public Object {
private:
	Stream::WriteRequest* buffer;
	char* p;
	bool bufferOverflow;
	std::string fieldInOverflow;

	inline static uint8_t getMessageChecksum(uint32_t size, uint16_t id) {
		uint8_t value = 0;

		value += size & 0xFF;
		value += (size >> 8) & 0xFF;
		value += (size >> 16) & 0xFF;
		value += (size >> 24) & 0xFF;

		value += id & 0xFF;
		value += (id >> 8) & 0xFF;

		return value;
	}

	void logBadPacketSize();

public:
	MessageBuffer(size_t size, int version);

	MessageBuffer(const void* data, size_t size, int version);

	~MessageBuffer();

	Stream::WriteRequest* getWriteRequest();
	const char* getData() const { return buffer->buffer.base; }
	char* getData() { return buffer->buffer.base; }
	uint32_t getSize() const { return buffer->buffer.len; }
	uint32_t getParsedSize() const { return uint32_t(p - getData()); }
	uint16_t getMessageId() const { return *reinterpret_cast<const uint16_t*>(buffer->buffer.base + 4); }
	std::string getFieldInOverflow() const { return fieldInOverflow; }

	bool checkFinalSize();
	bool checkPacketFinalSize() {
		uint32_t msgSize = *reinterpret_cast<const uint32_t*>(buffer->buffer.base);
		bool ok = !bufferOverflow && msgSize == getSize() && uint32_t(p - buffer->buffer.base) == msgSize;
		if(!ok) {
			logBadPacketSize();
		}
		return ok;
	}

	bool checkAvailableBuffer(const char* fieldName, size_t size) {
		if(bufferOverflow == false) {
			bool ok = size < 65536 && size_t(p + size - getData()) <= size_t(getSize());
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

	void writeHeader(uint32_t size, uint16_t id) {
		write<uint32_t>("size", size);
		write<uint16_t>("id", id);
		write<uint8_t>("msg_checksum", getMessageChecksum(size, id));
	}

	// Primitives
	template<typename T>
	typename std::enable_if<is_primitive<T>::value, void>::type write(const char* fieldName, T val) {
		if(checkAvailableBuffer(fieldName, sizeof(T))) {
			*reinterpret_cast<T*>(p) = val;
			p += sizeof(T);
		}
	}

	// Objects
	template<typename T>
	typename std::enable_if<!is_primitive<T>::value, void>::type write(const char* fieldName, const T& val) {
		val.serialize(this);
	}

	// String
	void writeString(const char* fieldName, const std::string& val, size_t maxSize);
	void writeDynString(const char* fieldName, const std::string& val, size_t count);

	// Fixed array of primitive
	template<typename T>
	typename std::enable_if<is_primitive<T>::value, void>::type writeArray(const char* fieldName,
	                                                                       const T* val,
	                                                                       size_t size) {
		if(checkAvailableBuffer(fieldName, sizeof(T) * size)) {
			memcpy(p, val, sizeof(T) * size);
			p += sizeof(T) * size;
		}
	}

	// Fixed array of primitive with cast
	template<typename T, typename U>
	typename std::enable_if<is_castable_primitive<T, U>::value, void>::type writeArray(const char* fieldName,
	                                                                                   const U* val,
	                                                                                   size_t size) {
		if(checkAvailableBuffer(fieldName, sizeof(T) * size)) {
			for(size_t i = 0; i < size; ++i) {
				*reinterpret_cast<T*>(p) = val[i];
				p += sizeof(T);
			}
		}
	}

	// Fixed array of object
	template<typename T>
	typename std::enable_if<!is_primitive<T>::value, void>::type writeArray(const char* fieldName,
	                                                                        const T* val,
	                                                                        size_t size) {
		for(size_t i = 0; i < size; i++) {
			write<T>(fieldName, val[i]);
		}
	}

	// Dynamic array of primitive
	template<typename T>
	typename std::enable_if<is_primitive<T>::value, void>::type writeDynArray(const char* fieldName,
	                                                                          const std::vector<T>& val,
	                                                                          uint32_t count) {
		size_t size = sizeof(T) * count;
		if(size && checkAvailableBuffer(fieldName, size)) {
			memcpy(p, &val[0], size);
			p += size;
		}
	}

	// Dynamic array of primitive with cast
	template<typename T, typename U>
	typename std::enable_if<is_castable_primitive<T, U>::value, void>::type writeDynArray(const char* fieldName,
	                                                                                      const std::vector<U>& val,
	                                                                                      uint32_t count) {
		size_t size = sizeof(T) * count;
		if(size && checkAvailableBuffer(fieldName, size)) {
			for(size_t i = 0; i < count; ++i) {
				*reinterpret_cast<T*>(p) = val[i];
				p += sizeof(T);
			}
		}
	}

	// Dynamic array of object or primitive with cast
	template<typename T>
	typename std::enable_if<!is_primitive<T>::value, void>::type writeDynArray(const char* fieldName,
	                                                                           const std::vector<T>& val,
	                                                                           uint32_t count) {
		for(size_t i = 0; i < count; i++)
			write<T>(fieldName, val[i]);
	}

	template<typename T>
	typename std::enable_if<is_primitive<T>::value, void>::type writeSize(const char* fieldName, T size) {
		write<T>(fieldName, size);
	}

	void pad(const char* fieldName, size_t size) {
		if(checkAvailableBuffer(fieldName, size)) {
			memset(p, 0, size);
			p += size;
		}
	}

	// Read functions /////////////////////////

	void readHeader(uint16_t& id) {
		discard("size", 4);
		read<uint16_t>("id", id);
		discard("msg_checksum", 1);
	}

	// Primitives via arg
	template<typename T, typename U>
	typename std::enable_if<is_primitive<U>::value, void>::type read(const char* fieldName, U& val) {
		if(checkAvailableBuffer(fieldName, sizeof(T))) {
			val = (U) * reinterpret_cast<T*>(p);
			p += sizeof(T);
		}
	}

	// Objects
	template<typename T>
	typename std::enable_if<!is_primitive<T>::value, void>::type read(const char* fieldName, T& val) {
		val.deserialize(this);
	}

	// String
	void readString(const char* fieldName, std::string& val, size_t size);
	void readDynString(const char* fieldName, std::string& val, uint32_t sizeToRead, bool hasNullTerminator);
	void readEndString(const char* fieldName, std::string& val, bool hasNullTerminator);

	// Fixed array of primitive
	template<typename T>
	typename std::enable_if<is_primitive<T>::value, void>::type readArray(const char* fieldName, T* val, size_t size) {
		if(checkAvailableBuffer(fieldName, sizeof(T) * size)) {
			memcpy(val, p, sizeof(T) * size);
			p += sizeof(T) * size;
		}
	}

	// Fixed array of primitive with cast
	template<typename T, typename U>
	typename std::enable_if<is_castable_primitive<T, U>::value, void>::type readArray(const char* fieldName,
	                                                                                  U* val,
	                                                                                  size_t size) {
		if(checkAvailableBuffer(fieldName, sizeof(T) * size)) {
			for(size_t i = 0; i < size; i++) {
				val[i] = (U) * reinterpret_cast<T*>(p);
				p += sizeof(T);
			}
		}
	}

	// Fixed array of objects
	template<typename T>
	typename std::enable_if<!is_primitive<T>::value, void>::type readArray(const char* fieldName, T* val, size_t size) {
		for(size_t i = 0; i < size; i++) {
			read<T>(fieldName, val[i]);
		}
	}

	// Dynamic array of primitive
	template<typename T>
	typename std::enable_if<is_primitive<T>::value, void>::type readDynArray(const char* fieldName,
	                                                                         std::vector<T>& val,
	                                                                         uint32_t sizeToRead) {
		size_t size = sizeof(T) * sizeToRead;
		if(size && checkAvailableBuffer(fieldName, size)) {
			val.assign(reinterpret_cast<T*>(p), reinterpret_cast<T*>(p) + sizeToRead);
			p += size;
		} else {
			val.clear();
		}
	}

	// Dynamic array of primitive with cast
	template<typename T, typename U>
	typename std::enable_if<is_castable_primitive<T, U>::value, void>::type readDynArray(const char* fieldName,
	                                                                                     std::vector<U>& val,
	                                                                                     uint32_t sizeToRead) {
		size_t size = sizeof(T) * sizeToRead;
		val.clear();
		if(size && checkAvailableBuffer(fieldName, size)) {
			val.reserve(sizeToRead);
			for(size_t i = 0; i < sizeToRead; i++) {
				val.push_back((U) * reinterpret_cast<T*>(p));
				p += sizeof(T);
			}
		}
	}

	// Dynamic array of object
	template<typename T>
	typename std::enable_if<!is_primitive<T>::value, void>::type readDynArray(const char* fieldName,
	                                                                          std::vector<T>& val,
	                                                                          uint32_t sizeToRead) {
		val.resize(sizeToRead, T{});

		auto it = val.begin();
		auto itEnd = val.end();
		for(; it != itEnd; ++it)
			read<T>(fieldName, *it);
	}

	// End array with primitive, read to the end of stream
	template<typename T>
	typename std::enable_if<is_primitive<T>::value, void>::type readEndArray(const char* fieldName,
	                                                                         std::vector<T>& val) {
		size_t remainingSize = 0;

		if(getSize() > getParsedSize())
			remainingSize = getSize() - getParsedSize();

		if(checkAvailableBuffer(fieldName, remainingSize)) {
			if(remainingSize > 0)
				val.assign(p, remainingSize);
			else
				val.clear();
			p += remainingSize;
		}
	}

	// End array, read to the end of stream
	template<typename T>
	typename std::enable_if<!is_primitive<T>::value, void>::type readEndArray(const char* fieldName,
	                                                                          std::vector<T>& val) {
		// While there are non parsed bytes and the read actually read something, continue
		uint32_t lastParsedSize = UINT32_MAX;

		if(getSize() > getParsedSize())
			val.reserve(getSize() - getParsedSize());

		while(lastParsedSize != getParsedSize() && getParsedSize() < getSize()) {
			lastParsedSize = getParsedSize();
			val.emplace_back();
			T& newItem = val.back();
			read<T>(fieldName, newItem);
		}
	}

	// read size for objects (std:: containers)
	template<typename T>
	typename std::enable_if<is_primitive<T>::value, void>::type readSize(const char* fieldName, uint32_t& val) {
		read<T>(fieldName, val);
	}

	void discard(const char* fieldName, size_t size) {
		if(checkAvailableBuffer(fieldName, size)) {
			p += size;
		}
	}
};

#endif  // MESSAGEBUFFER_H
