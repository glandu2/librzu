#ifndef CLIENTGAMESESSION_H
#define CLIENTGAMESESSION_H

#include "EncryptedSession.h"
#include "PacketSession.h"

class ClientAuthSession;

class RZU_EXTERN ClientGameSession : public EncryptedSession<PacketSession> {
public:
	ClientGameSession(int version);

	void setAuthSession(ClientAuthSession* auth) { this->auth = auth; }

protected:
	virtual void onGameConnected() = 0;
	virtual void onGamePacketReceived(const TS_MESSAGE* packet) = 0;
	virtual void onGameDisconnected(bool causedByRemote) = 0;

private:
	virtual EventChain<SocketSession> onConnected() override final;
	virtual EventChain<PacketSession> onPacketReceived(const TS_MESSAGE* packet) override final;
	virtual EventChain<SocketSession> onDisconnected(bool causedByRemote) override final;

protected:
	ClientAuthSession* auth;
};

#endif  // CLIENTGAMESESSION_H
