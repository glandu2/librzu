#ifndef CLIENTGAMESESSION_H
#define CLIENTGAMESESSION_H

#include "PacketSession.h"
#include "EncryptedSession.h"

class ClientAuthSessionCommon;
class ClientAuthSession;

class RZU_EXTERN ClientGameSession : public EncryptedSession<PacketSession>
{
public:
	ClientGameSession(int version);

	void setAuthSession(ClientAuthSession* auth) { this->auth = auth; }

protected:
	virtual void onGameConnected() = 0;
	virtual void onGamePacketReceived(const TS_MESSAGE *packet) = 0;
	virtual void onGameDisconnected() = 0;

private:
	EventChain<SocketSession> onConnected();
	EventChain<PacketSession> onPacketReceived(const TS_MESSAGE *packet);
	EventChain<SocketSession> onDisconnected(bool causedByRemote);

protected:
	ClientAuthSession* auth;
	int version;
};

#endif // CLIENTGAMESESSION_H
