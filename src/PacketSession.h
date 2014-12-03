#ifndef PACKETSESSION_H
#define PACKETSESSION_H

#include "SocketSession.h"
#include "Packets/PacketBaseMessage.h"
#include "Stream.h"

class RappelzServerCommon;

class RAPPELZLIB_EXTERN PacketSession : public SocketSession
{
	DECLARE_CLASS(PacketSession)

public:
	typedef void (*CallbackFunction)(IListener* instance, PacketSession* server, const TS_MESSAGE* packetData);
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
	virtual void assignStream(Stream* stream);

	virtual void onPacketReceived(const TS_MESSAGE* packet) {}


	void sendPacket(const TS_MESSAGE* data);

	void addPacketListener(uint16_t packetId, IListener* instance, CallbackFunction onPacketReceivedCallback);
	void removePacketListener(uint16_t packetId, IListener* instance);
	void setUnknownPacketListener(IListener* instance, CallbackFunction onPacketReceivedCallback);

protected:
	virtual ~PacketSession() {}
	void addPacketsToListen(int packetsIdNum, int firstPacketId, ...);

	void onDataReceived();

	static void onPacketReceivedStatic(IListener* instance, PacketSession* clientSocket, const TS_MESSAGE* packet);
	static void stateChanged(IListener* instance, Stream* socket, Stream::State oldState, Stream::State newState);
	static void socketError(IListener* instance, Stream* socket, int errnoValue);

	void dispatchPacket(const TS_MESSAGE* packetData);
	void logPacket(bool outgoing, const TS_MESSAGE* msg);

private:

	IDelegateHash<uint16_t, CallbackFunction> packetListeners;

	InputBuffer inputBuffer;
};

#endif // PACKETSESSION_H
