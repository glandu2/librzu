#include "MessageBuffer.h"
#include <algorithm>
#include "Core/Utils.h"

MessageBuffer::MessageBuffer(size_t size, int version) : StructSerializer(version) {
	buffer = Stream::WriteRequest::create(size);
	p = buffer->buffer.base;
	bufferOverflow = false;
}

MessageBuffer::MessageBuffer(const void* data, size_t size, int version) : StructSerializer(version) {
	buffer = Stream::WriteRequest::createFromExisting(const_cast<char*>(static_cast<const char*>(data)), size);
	p = buffer->buffer.base;
	bufferOverflow = false;
}

MessageBuffer::~MessageBuffer() {
	if(buffer) {
		Stream::WriteRequest::destroy(buffer);
	}
}

Stream::WriteRequest *MessageBuffer::getWriteRequest() {
	Stream::WriteRequest* ret = buffer;
	buffer = nullptr;
	return ret;
}

bool MessageBuffer::checkFinalSize() {
	if(bufferOverflow) {
		log(LL_Error, "Serialization overflow: version: %d, buffer size: %d, offset: %d, field: %s\n",
		    getVersion(), getSize(), uint32_t(p - buffer->buffer.base), getFieldInOverflow().c_str());
	}
	return !bufferOverflow;
}

bool MessageBuffer::checkPacketFinalSize() {
	uint32_t msgSize = *reinterpret_cast<const uint32_t*>(buffer->buffer.base);
	bool ok = !bufferOverflow && msgSize == getSize() && uint32_t(p - buffer->buffer.base) == msgSize;
	if(!ok) {
		if(bufferOverflow) {
			log(LL_Error, "Packet read/write overflow: id: %d, packet size: %d, buffer size: %d, offset: %d, field: %s\n",
			    getMessageId(), msgSize, getSize(), uint32_t(p - buffer->buffer.base), bufferOverflow ? getFieldInOverflow().c_str() : "<no overflow>");
		} else if(msgSize != getSize()) {
			log(LL_Error, "Packet size is not buffer size: id: %d, packet size: %d, buffer size: %d, offset: %d\n",
			    getMessageId(), msgSize, getSize(), uint32_t(p - buffer->buffer.base));
		} else if(uint32_t(p - buffer->buffer.base) != msgSize) {
			log(LL_Error, "Packet was not fully read/written: id: %d, packet size: %d, buffer size: %d, offset: %d\n",
			    getMessageId(), msgSize, getSize(), uint32_t(p - buffer->buffer.base));
		} else {
			log(LL_Error, "Packet has invalid data: id: %d, packet size: %d, buffer size: %d, offset: %d, field: %s\n",
			    getMessageId(), msgSize, getSize(), uint32_t(p - buffer->buffer.base), bufferOverflow ? getFieldInOverflow().c_str() : "<no overflow>");
		}

	}
	return ok;
}

void MessageBuffer::writeString(const char *fieldName, const std::string &val, size_t maxSize) {
	if(checkAvailableBuffer(fieldName, maxSize)) {
		size_t stringSize = std::min(val.size(), maxSize-1); // keep room for nul terminator (truncate val if too long)
		memcpy(p, val.c_str(), stringSize);
		memset(p + stringSize, 0, maxSize - stringSize);
		p += maxSize;
	}
}

void MessageBuffer::writeDynString(const char *fieldName, const std::string &val, size_t count) {
	if(checkAvailableBuffer(fieldName, count)) {
		memcpy(p, val.c_str(), count);
		p += count;
	}
}

void MessageBuffer::readString(const char *fieldName, std::string &val, size_t maxSize) {
	if(checkAvailableBuffer(fieldName, maxSize)) {
		val = Utils::convertToString(p, (int)maxSize-1);
		p += maxSize;
	}
}

void MessageBuffer::readDynString(const char *fieldName, std::string &val, uint32_t sizeToRead, bool hasNullTerminator) {
	if(checkAvailableBuffer(fieldName, sizeToRead)) {
		if(sizeToRead > 0)
			val.assign(p, sizeToRead - hasNullTerminator);
		else
			val.clear();
		p += sizeToRead;
	}
}

void MessageBuffer::readEndString(const char* fieldName, std::string& val, bool hasNullTerminator)
{
	size_t remainingSize = getSize() - getParsedSize();
	if(checkAvailableBuffer(fieldName, remainingSize)) {
		if(remainingSize > 0)
			val.assign(p, remainingSize - hasNullTerminator);
		else
			val.clear();
		p += remainingSize;
	}
}
