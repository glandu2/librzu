#ifndef PACKETS_PACKETBASEMESSAGE_H
#define PACKETS_PACKETBASEMESSAGE_H

#include "Packet/PacketEpics.h"
#include <stdint.h>
#include <string.h>
#include <type_traits>

#ifndef __GNUC__
#pragma warning(disable : 4200)  // array [0] extension
#pragma warning(disable : 4624)  // no default constructor/destructor when private in base class
#endif

#pragma pack(push, 1)

struct TS_MESSAGE_WNA;

struct TS_MESSAGE {
	uint32_t size;
	uint16_t id;
	int8_t msg_check_sum;

	template<typename MessageType>
	static typename std::enable_if<std::is_base_of<TS_MESSAGE, MessageType>::value, void>::type initMessage(
	    MessageType* msg) {
		memset(msg, 0, sizeof(MessageType));
		msg->size = sizeof(MessageType);
		msg->id = MessageType::packetID;
		msg->msg_check_sum = checkMessage(msg);
	}
	static int8_t checkMessage(const TS_MESSAGE* msg) {
		char* p = (char*) &(msg->size);
		char* end = (char*) &(msg->msg_check_sum);
		int value = 0;

		while(p != end) {
			value += *p;
			p++;
		}

		return value;
	}

	template<typename PacketType> static PacketType* create(uint32_t size = sizeof(PacketType)) {
		PacketType* msg = (PacketType*) new char[size];
		memset(msg, 0, size);
		msg->size = size;
		msg->id = PacketType::packetID;
		msg->msg_check_sum = checkMessage(msg);
		return msg;
	}

	static void destroy(TS_MESSAGE* msg) { delete[](char*) msg; }

	template<class T, class U>
	void process(U* instance, void (U::*processFunction)(const T*), packet_version_t version) const;

	template<class T, class U, typename... Args>
	void process(U* instance,
	             void (U::*processFunction)(const T*, Args...),
	             packet_version_t version,
	             Args... args) const;

	template<class T> bool process(T& packet, packet_version_t version) const;
};

// Special struct to prevent copy of server->client packet structs
// WNA stand for WITH NESTED ARRAY but shorter
struct TS_MESSAGE_WNA : public TS_MESSAGE {
	template<typename PacketType, typename NestedArray> static PacketType* create(int nestedElementCount = 0) {
		PacketType* msg = (PacketType*) new char[sizeof(PacketType) + sizeof(NestedArray) * nestedElementCount];
		int size = sizeof(PacketType) + sizeof(NestedArray) * nestedElementCount;
		memset(msg, 0, size);
		msg->size = size;
		msg->id = PacketType::packetID;
		msg->msg_check_sum = checkMessage(msg);
		return msg;
	}

	template<typename PacketType, typename NestedArray1, typename NestedArray2>
	static NestedArray2* getSecondArrayPtr(TS_MESSAGE_WNA* msg, int nestedArray1ElementCount) {
		return reinterpret_cast<NestedArray2*>(reinterpret_cast<char*>(msg) + sizeof(PacketType) +
		                                       nestedArray1ElementCount * sizeof(NestedArray1));
	}

private:
	TS_MESSAGE_WNA() {}
	~TS_MESSAGE_WNA() {}
	TS_MESSAGE_WNA(TS_MESSAGE_WNA const&);             // undefined
	TS_MESSAGE_WNA& operator=(TS_MESSAGE_WNA const&);  // undefined
};

#pragma pack(pop)

#include "MessageBuffer.h"

template<class T, class U>
void TS_MESSAGE::process(U* instance, void (U::*processFunction)(const T*), packet_version_t version) const {
	T packet;
	MessageBuffer buffer((void*) this, this->size, version);

	packet.deserialize(&buffer);
	if(buffer.checkPacketFinalSize()) {
		(instance->*processFunction)(&packet);
	}
}

template<class T, class U, typename... Args>
void TS_MESSAGE::process(U* instance,
                         void (U::*processFunction)(const T*, Args...),
                         packet_version_t version,
                         Args... args) const {
	T packet;
	MessageBuffer buffer((void*) this, this->size, version);

	packet.deserialize(&buffer);
	if(buffer.checkPacketFinalSize()) {
		(instance->*processFunction)(&packet, args...);
	}
}

template<class T> bool TS_MESSAGE::process(T& packet, packet_version_t version) const {
	MessageBuffer buffer((void*) this, this->size, version);

	packet.deserialize(&buffer);
	return buffer.checkPacketFinalSize();
}

#endif  // PACKETS_PACKETBASEMESSAGE_H
