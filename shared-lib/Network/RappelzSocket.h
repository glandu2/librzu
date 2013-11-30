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

	virtual bool IFACECALLCONV connect(const std::string & hostName, uint16_t port);
	void sendPacket(const TS_MESSAGE* data);

	DelegateRef addPacketListener(uint16_t packetId, void* instance, CallbackFunction onPacketReceivedCallback);

	const std::string& getHost() { return host; }
	uint16_t getPort() { return port; }

protected:
	static void dataReceived(void* instance, ISocket* socket);
	static void stateChanged(void* instance, ISocket* socket, ISocket::State oldState, ISocket::State newState);
	static void socketError(void* instance, ISocket* socket, int errnoValue);

	void dispatchPacket(const TS_MESSAGE* packetData);

private:
	IDelegateHash<uint16_t, RappelzSocket*, const TS_MESSAGE*> packetListeners;
	std::string host;
	uint16_t port;

	InputBuffer inputBuffer;
};

#endif // RAPPELZSOCKET_H
