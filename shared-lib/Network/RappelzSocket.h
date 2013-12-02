#ifndef RAPPELZSOCKET_H
#define RAPPELZSOCKET_H

#include "EncryptedSocket.h"
#include "Interfaces/IDelegate.h"
#include "Packets/PacketBaseMessage.h"

//Pour le syntax higlighting de intxx_t
#ifdef __GNUC__
# include <stdint-gcc.h>
#else
# include <stdint.h>
#endif

class RappelzSocket : public EncryptedSocket, private ICallbackGuard
{
public:
	typedef void (*CallbackFunction)(void* instance, RappelzSocket* server, const TS_MESSAGE* packetData);
	static const uint16_t ALL_PACKETS = 0xFFFE;

private:
	static const uint32_t initialInputBufferSize = 16384;
	struct InputBuffer {
		uint8_t* buffer;
		uint32_t bufferSize;
		uint32_t currentMessageSize;
	};

public:
	RappelzSocket();
	virtual ~RappelzSocket();

	void sendPacket(const TS_MESSAGE* data);

	DelegateRef addPacketListener(uint16_t packetId, void* instance, CallbackFunction onPacketReceivedCallback);

protected:
	static void dataReceived(void* instance, Socket* socket);
	static void stateChanged(void* instance, Socket* socket, Socket::State oldState, Socket::State newState);
	static void socketError(void* instance, Socket* socket, int errnoValue);

	void dispatchPacket(const TS_MESSAGE* packetData);

private:
	IDelegateHash<uint16_t, RappelzSocket*, const TS_MESSAGE*> packetListeners;

	InputBuffer inputBuffer;
};

#endif // RAPPELZSOCKET_H
