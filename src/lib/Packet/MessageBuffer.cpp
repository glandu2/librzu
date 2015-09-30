#include "MessageBuffer.h"
#include "Core/Utils.h"

MessageBuffer::MessageBuffer(size_t size, int version) {
	buffer = Stream::WriteRequest::create(size);
	p = buffer->buffer.base;
	this->version = version;
	bufferOverflow = false;
}

MessageBuffer::MessageBuffer(const void *data, size_t size, int version) {
	buffer = Stream::WriteRequest::createFromExisting(const_cast<char*>(static_cast<const char*>(data)), size);
	p = buffer->buffer.base;
	this->version = version;
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
	uint32_t msgSize = *reinterpret_cast<const uint32_t*>(buffer->buffer.base);
	bool ok = !bufferOverflow && msgSize == getSize() && uint32_t(p - buffer->buffer.base) == msgSize;
	if(!ok)
		log(LL_Error, "Packet has invalid data: id: %d, size: %d, field: %s\n", getMessageId(), getSize(), getFieldInOverflow().c_str());
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

void MessageBuffer::readString(const char *fieldName, std::string &val, size_t maxSize) {
	if(checkAvailableBuffer(fieldName, maxSize)) {
		val = Utils::convertToString(p, (int)maxSize-1);
		p += maxSize;
	}
}
