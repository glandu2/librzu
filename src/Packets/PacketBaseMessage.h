#ifndef PACKETBASEMESSAGE_H
#define PACKETBASEMESSAGE_H

#include <string.h>

#include "../BufferReader.h"
#include "../BufferWriter.h"

#ifdef __GNUC__
#include <stdint-gcc.h>
#else
#include <stdint.h>
#endif

#ifndef __GNUC__
#pragma warning(disable:4200)  //array [0] extension
#pragma warning(disable:4624)  //no default constructor/destructor when private in base class
#endif

#pragma pack(push, 1)

struct TS_MESSAGE_WNA;

struct TS_MESSAGE {
	uint32_t size;
	uint16_t id;
	int8_t msg_check_sum;

	template<typename MessageType>
	static void initMessage(MessageType* msg) {
		memset(msg, 0, sizeof(MessageType));
		msg->size = sizeof(MessageType);
		msg->id = MessageType::packetID;
		msg->msg_check_sum = checkMessage(msg);
	}
	static int8_t checkMessage(const TS_MESSAGE* msg) {
		char *p = (char*)&(msg->size);
		char *end = (char*)&(msg->msg_check_sum);
		int value = 0;

		while(p != end) {
			value += *p;
			p++;
		}

		return value;
	}

	template<typename PacketType>
	static PacketType* create(uint32_t size = sizeof(PacketType)) {
		PacketType* msg = (PacketType*) new char[size];
		memset(msg, 0, size);
		msg->size = size;
		msg->id = PacketType::packetID;
		msg->msg_check_sum = checkMessage(msg);
		return msg;
	}

	static void destroy(TS_MESSAGE* msg) {
		delete[] (char*)msg;
	}
};

//Special message for connection/disconnection events and other client side errors
//This special packet is never send or received through any network.
struct TS_CC_EVENT : public TS_MESSAGE {
	enum ClientError {
		CE_ServerConnected,
		CE_ServerDisconnected,
		CE_ServerConnectionLost,
		CE_ServerUnreachable
	} event;
	static const uint16_t packetID = 0xFFFF;
};


//Special struct to prevent copy of server->client packet structs
//WNA stand for WITH NESTED ARRAY but shorter
struct TS_MESSAGE_WNA : public TS_MESSAGE {
		template<typename PacketType, typename NestedArray>
		static PacketType* create(int nestedElementCount = 0) {
			PacketType* msg = (PacketType*) new char[sizeof(PacketType) + sizeof(NestedArray)*nestedElementCount];
			int size = sizeof(PacketType) + sizeof(NestedArray)*nestedElementCount;
			memset(msg, 0, size);
			msg->size = size;
			msg->id = PacketType::packetID;
			msg->msg_check_sum = checkMessage(msg);
			return msg;
		}

		template<typename PacketType, typename NestedArray1, typename NestedArray2>
		static NestedArray2* getSecondArrayPtr(TS_MESSAGE_WNA* msg, int nestedArray1ElementCount) {
			return reinterpret_cast<NestedArray2*>(reinterpret_cast<char*>(msg) + sizeof(PacketType) + nestedArray1ElementCount*sizeof(NestedArray1));
		}

		private:
		   TS_MESSAGE_WNA() {}
		   ~TS_MESSAGE_WNA() {}
		   TS_MESSAGE_WNA( TS_MESSAGE_WNA const & );            // undefined
		   TS_MESSAGE_WNA& operator=( TS_MESSAGE_WNA const & ); // undefined
};


// New serialization

struct TS_MESSAGE_BASE {
	uint16_t id;

	virtual int getSize() const = 0;
	virtual void serialize(BufferWriter* buffer) const {}
	virtual void deserialize(BufferReader* buffer) {}
};

struct TS_MESSAGE_SERIALIZABLE : TS_MESSAGE_BASE {
	void serializeHeader(BufferWriter* buffer) const {
		buffer->write32(buffer->getSize());
		buffer->write16(id);
		buffer->write8(checkMessage(buffer->getSize(), id));
	}
private:
	static uint8_t checkMessage(uint32_t size, uint16_t id) {
		uint8_t value = 0;

		value += size & 0xFF;
		value += (size >> 8) & 0xFF;
		value += (size >> 16) & 0xFF;
		value += (size >> 24) & 0xFF;

		value += id & 0xFF;
		value += (id >> 8) & 0xFF;

		return value;
	}
};

struct TS_MESSAGE_HEADER : TS_MESSAGE_BASE {
	BufferReader* buffer;

	void deserializeHeader(BufferReader* buffer) {
		this->buffer = buffer;

		buffer->read32();
		buffer->read16(&id);
		buffer->read8();
	}

	template<class TS_TYPE>
	operator TS_TYPE() {
		TS_TYPE val;
		val.id = id;
		val.buffer = buffer;

		val.deserialize(buffer);
		return val;
	}
};

#pragma pack(pop)

#endif // PACKETBASEMESSAGE_H
