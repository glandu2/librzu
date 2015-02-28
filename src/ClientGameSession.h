#ifndef CLIENTGAMESESSION_H
#define CLIENTGAMESESSION_H

#include "PacketSession.h"
#include "EncryptedSession.h"

class ClientAuthSessionCommon;
class ClientAuthSession;

class LIB_EXTERN ClientGameSession : public EncryptedSession<PacketSession>
{
public:
	ClientGameSession();

	void setAuthSession(ClientAuthSession* auth) { this->auth = auth; }

protected:
	virtual void onGameConnected() = 0;
	virtual void onGamePacketReceived(const TS_MESSAGE *packet) = 0;
	virtual void onGameDisconnected() = 0;

private:
	void onConnected();
	void onPacketReceived(const TS_MESSAGE *packet);
	void onDisconnected(bool causedByRemote);

protected:
	ClientAuthSession* auth;
};

#endif // CLIENTGAMESESSION_H
