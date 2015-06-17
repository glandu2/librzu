#ifndef PACKETSESSION_H
#define PACKETSESSION_H

#include "SocketSession.h"
#include "Packets/PacketBaseMessage.h"
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

	void sendPacket(const TS_MESSAGE* data);

	virtual bool hasCustomPacketLogger() { return true; }
	static bool hasCustomPacketLoggerStatic() { return true; }

protected:

	virtual void onPacketReceived(const TS_MESSAGE* packet) {}
	virtual void onStateChanged(Stream::State oldState, Stream::State newState, bool causedByRemote);
	virtual void onError(int err);

	void dispatchPacket(const TS_MESSAGE* packetData);
	virtual void logPacket(bool outgoing, const TS_MESSAGE* msg);

private:
	void onDataReceived();

private:
	InputBuffer inputBuffer;
};

#endif // PACKETSESSION_H
