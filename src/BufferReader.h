#ifndef BUFFERREADER_H
#define BUFFERREADER_H

#include <vector>
#include <string>
#include <stdint.h>
#include <string.h>

class BufferReader
{
public:
	size_t size;
	char* buffer;
	char* p;
	int version;

	BufferReader(int version = 0) : size(0), buffer(nullptr), p(nullptr), version(version) {}

	void start(size_t size) {
		this->size = size;
		buffer = new char[size];
		p = buffer;
	}

	~BufferReader() { delete[] buffer; }

	void reset() {
		p = buffer;
	}

	#define DEFINE_READ(_NAME, _TYPE) \
		void _NAME(_TYPE* val) { *val = *reinterpret_cast<_TYPE*>(p); p += sizeof(_TYPE); } \
		void _NAME##Array(_TYPE* val, int size) { memcpy(val, p, size*sizeof(_TYPE)); p += size*sizeof(_TYPE); } \
		void _NAME##Array(std::vector<_TYPE>& val, int size) { _TYPE* valPtr = reinterpret_cast<_TYPE*>(p); val.assign(valPtr, valPtr + size); p += size*sizeof(_TYPE); }
	DEFINE_READ(read8, uint8_t)
	DEFINE_READ(read16, uint16_t)
	DEFINE_READ(read32, uint32_t)
	DEFINE_READ(read64, uint64_t)
	DEFINE_READ(read8, int8_t)
	DEFINE_READ(read16, int16_t)
	DEFINE_READ(read32, int32_t)
	DEFINE_READ(read64, int64_t)

	#define DEFINE_READ_UNIQUE(_NAME, _TYPE) \
		template<class TS_TYPE> void _NAME##Size(std::vector<TS_TYPE>& val, int size) { _TYPE* valPtr = reinterpret_cast<_TYPE*>(p); val.resize(*valPtr); p += sizeof(_TYPE); } \
		void _NAME() { p += sizeof(_TYPE); }
	DEFINE_READ_UNIQUE(read8, uint8_t)
	DEFINE_READ_UNIQUE(read16, uint16_t)
	DEFINE_READ_UNIQUE(read32, uint32_t)
	DEFINE_READ_UNIQUE(read64, uint64_t)

	void readStr(char* val, int size) { read8Array(reinterpret_cast<uint8_t*>(val), size); }
	void readStr(std::string& val, int size) { val.assign(p, size); p += size; }

	template<class TS_TYPE>
	void read(std::vector<TS_TYPE>& obj) {
		auto it = obj.begin();
		auto itEnd = obj.end();
		for(; it != itEnd; ++it)
			(*it).deserialize(this);
	}

	template<class TS_TYPE>
	void read(TS_TYPE& obj) {
		obj.deserialize(this);
	}

	int getSize() { return (int)size; }
};

#endif // BUFFERREADER_H
