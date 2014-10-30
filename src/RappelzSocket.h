#ifndef RAPPELZSOCKET_H
#define RAPPELZSOCKET_H

#include "EncryptedSocket.h"
#include "IDelegate.h"
#include "Packets/PacketBaseMessage.h"

//Pour le syntax higlighting de intxx_t
#ifdef __GNUC__
# include <stdint-gcc.h>
#else
# include <stdint.h>
#endif

class RAPPELZLIB_EXTERN RappelzSocket : public EncryptedSocket, private IListener
{
	DECLARE_CLASS(RappelzSocket)

public:
	typedef void (*CallbackFunction)(IListener* instance, RappelzSocket* server, const TS_MESSAGE* packetData);
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
	RappelzSocket(uv_loop_t* uvLoop, Mode mode);
	virtual ~RappelzSocket();

	void sendPacket(const TS_MESSAGE* data);
	/*void sendPacket(const TS_MESSAGE_SERIALIZABLE* data);*/

	void addPacketListener(uint16_t packetId, IListener* instance, CallbackFunction onPacketReceivedCallback);
	void removePacketListener(uint16_t packetId, IListener* instance);
	void setUnknownPacketListener(IListener* instance, CallbackFunction onPacketReceivedCallback);

protected:
	static void dataReceived(IListener *instance, Socket* socket);
	static void stateChanged(IListener* instance, Socket* socket, Socket::State oldState, Socket::State newState);
	static void socketError(IListener* instance, Socket* socket, int errnoValue);

	void dispatchPacket(const TS_MESSAGE* packetData);
	void logPacket(bool outgoing, const TS_MESSAGE* msg);

private:
	IDelegateHash<uint16_t, CallbackFunction> packetListeners;
	Callback<CallbackFunction> defaultPacketListener; //called when packet id is not known

	InputBuffer inputBuffer;
};

#endif // RAPPELZSOCKET_H
