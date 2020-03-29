#ifndef PACKETSESSION_H
#define PACKETSESSION_H

#include "Packet/MessageBuffer.h"
#include "Packet/PacketBaseMessage.h"
#include "Packet/PacketEpics.h"
#include "Packet/PacketStructsName.h"
#include "SocketSession.h"
#include "Stream/Stream.h"

class SessionServerCommon;

class RZU_EXTERN PacketSession : public SocketSession {
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
	PacketSession(SessionType sessionType, SessionPacketOrigin packetOrigin, int version);
	virtual ~PacketSession();

	template<class T>
	typename std::enable_if<!std::is_pointer<T>::value, void>::type sendPacket(const T& data,
	                                                                           packet_version_t versionToSend) {
		MessageBuffer buffer(data.getSize(versionToSend), versionToSend);
		data.serialize(&buffer);
		if(buffer.checkPacketFinalSize() == false) {
			log(LL_Error,
			    "Wrong packet buffer size, id: %d, size: %d, field: %s\n",
			    buffer.getMessageId(),
			    buffer.getSize(),
			    buffer.getFieldInOverflow().c_str());
			abortSession();
		} else {
			if(logPacketEnabled()) {
				logPacketWithoutJson(true, (const TS_MESSAGE*) buffer.getData(), data.getName());
				logPacketJson(&data, packetVersion, true);
			}

			write(buffer.getWriteRequest());
		}
	}
	template<class T> typename std::enable_if<!std::is_pointer<T>::value, void>::type sendPacket(const T& data) {
		sendPacket<T>(data, packetVersion);
	}

	void sendPacket(const TS_MESSAGE* data);

	virtual bool hasCustomPacketLogger() { return true; }
	static bool hasCustomPacketLoggerStatic() { return true; }

protected:
	EventChain<SocketSession> onConnected();

	virtual EventChain<PacketSession> onPacketReceived(const TS_MESSAGE* packet) { return EventChain<PacketSession>(); }

	void dispatchPacket(const TS_MESSAGE* packetData);
	bool logPacketEnabled();
	virtual void logPacket(bool outgoing, const TS_MESSAGE* msg);
	void logPacketWithoutJson(bool outgoing, const TS_MESSAGE* msg, const char* packetName);

private:
	EventChain<SocketSession> onDataReceived();

	template<class Packet> struct PrintPacketFunctor;
	template<class Packet> RZU_EXTERN void logPacketJson(const Packet* packet, packet_version_t version, bool outgoing);

protected:
	packet_version_t packetVersion;

private:
	SessionType sessionType;
	SessionPacketOrigin packetOrigin;
	InputBuffer inputBuffer;

	cval<bool>& dumpRawPackets;
	cval<bool>& dumpJsonPackets;
};

#endif  // PACKETSESSION_H
