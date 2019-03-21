#ifndef CLIENTGAMESESSION_H
#define CLIENTGAMESESSION_H

#include "EncryptedSession.h"
#include "PacketSession.h"

class ClientAuthSessionCommon;
class ClientAuthSession;

class RZU_EXTERN ClientGameSession : public EncryptedSession<PacketSession> {
public:
	ClientGameSession(int version);

	void setAuthSession(ClientAuthSession* auth) { this->auth = auth; }

	using EncryptedSession<PacketSession>::sendPacket;
	template<class T> typename std::enable_if<!std::is_pointer<T>::value, void>::type sendPacket(const T& data) {
		EncryptedSession<PacketSession>::sendPacket(data, version);
	}

protected:
	virtual void onGameConnected() = 0;
	virtual void onGamePacketReceived(const TS_MESSAGE* packet) = 0;
	virtual void onGameDisconnected(bool causedByRemote) = 0;

private:
	EventChain<SocketSession> onConnected();
	EventChain<PacketSession> onPacketReceived(const TS_MESSAGE* packet);
	EventChain<SocketSession> onDisconnected(bool causedByRemote);

protected:
	ClientAuthSession* auth;
	int version;
};

#endif  // CLIENTGAMESESSION_H
