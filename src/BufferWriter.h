#ifndef BUFFERWRITER_H
#define BUFFERWRITER_H

#include "Socket.h"
#include <vector>
#include <string.h>

class BufferWriter
{
public:
	Socket::WriteRequest* writeRequest;
	char* p;
	int version;

	BufferWriter(size_t size, int version) : version(version) {
		writeRequest = Socket::WriteRequest::create(size);
		p = writeRequest->buffer.base;
	}

	char* getData() { return writeRequest->buffer.base; }
	int getSize() { return writeRequest->buffer.len; }

	void reset() {
		if(writeRequest)
			p = writeRequest->buffer.base;
		else
			p = nullptr;
	}

	#define DEFINE_WRITE(_NAME, _TYPE) \
		void _NAME(_TYPE val) { *reinterpret_cast<_TYPE*>(p) = val; p += sizeof(_TYPE); } \
		void _NAME##Array(const _TYPE* val, int size) { memcpy(p, val, size*sizeof(_TYPE)); p += size*sizeof(_TYPE); } \
		void _NAME##Array(const std::vector<_TYPE>& val, int size) { memcpy(p, &val[0], val.size()*sizeof(_TYPE)); p += size*sizeof(_TYPE); }
	DEFINE_WRITE(write8, uint8_t)
	DEFINE_WRITE(write16, uint16_t)
	DEFINE_WRITE(write32, uint32_t)
	DEFINE_WRITE(write64, uint64_t)
	DEFINE_WRITE(write8, int8_t)
	DEFINE_WRITE(write16, int16_t)
	DEFINE_WRITE(write32, int32_t)
	DEFINE_WRITE(write64, int64_t)

	void writeStr(const char* val, int size) { write8Array(reinterpret_cast<const uint8_t*>(val), size); }
	void writeStr(const std::string& val, int size) { writeStr(val.c_str(), size); }

	template<class TS_TYPE>
	void write(std::vector<TS_TYPE>& obj) {
		auto it = obj.begin();
		auto itEnd = obj.end();
		for(; it != itEnd; ++it)
			(*it).serialize(this);
	}

	template<class TS_TYPE>
	void write(TS_TYPE& obj) {
		obj.serialize(this);
	}

	bool checkFinalSize() {
		uint32_t msgSize = *reinterpret_cast<uint32_t*>(writeRequest->buffer.base);
		return uint32_t(p - writeRequest->buffer.base) == msgSize;
	}
};

#endif // BUFFERWRITER_H
