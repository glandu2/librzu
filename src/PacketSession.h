#ifndef PACKETSESSION_H
#define PACKETSESSION_H

#include "SocketSession.h"
#include "Packets/PacketBaseMessage.h"
#include "MessageBuffer.h"
#include "Stream.h"

class SessionServerCommon;

class RZU_EXTERN PacketSession : public SocketSession
{
	DECLARE_CLASS(PacketSession)

public:
	static const uint16_t ALL_PACKETS = 0xFFFE;
	static const uint32_t MAX_PACKET_SIZE = 65536;

private:
	static const uint32_t INITIAL_INPUT_BUFFERSIZE = 296;
	struct InputBuffer {
		uint8_t* buffer;
		uint32_t bufferSize;
		uint32_t currentMessageSize;
		bool discardPacket;
	};

public:
	PacketSession();
	virtual ~PacketSession();

	template<class T>
	void sendPacket(const T& data, int version) {
		MessageBuffer buffer(data.getSize(version), version);
		data.serialize(&buffer);
		if(buffer.checkFinalSize() == false) {
			error("Wrong packet buffer size, id: %d, size: %d, field: %s\n", buffer.getMessageId(), buffer.getSize(), buffer.getFieldInOverflow().c_str());
			abortSession();
		} else {
			sendPacket(reinterpret_cast<const TS_MESSAGE*>(buffer.getData()));
		}
	}

	void sendPacket(const TS_MESSAGE* data);

	template<class T, class U, class V>
	void process(MessageBuffer* buffer, void (U::*processFunction)(V)) {
		T packet;
		packet.deserialize(buffer);
		if(buffer->checkFinalSize()) {
			(static_cast<U*>(this)->*processFunction)(packet);
		} else {
			error("Received packet with invalid data: id: %d, size: %d, field: %s\n", buffer->getMessageId(), buffer->getSize(), buffer->getFieldInOverflow().c_str());
			abortSession();
		}
	}

	virtual bool hasCustomPacketLogger() { return true; }
	static bool hasCustomPacketLoggerStatic() { return true; }

protected:

	virtual void onPacketReceived(const TS_MESSAGE* packet) {}

	void dispatchPacket(const TS_MESSAGE* packetData);
	virtual void logPacket(bool outgoing, const TS_MESSAGE* msg);

private:
	void onDataReceived();

private:
	InputBuffer inputBuffer;
};

#endif // PACKETSESSION_H
