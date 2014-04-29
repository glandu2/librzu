#ifndef RAPPELZSESSION_H
#define RAPPELZSESSION_H

#include "SocketSession.h"
#include "RappelzSocket.h"
#include <list>

class RappelzServerCommon;

class RAPPELZLIB_EXTERN RappelzSession : public SocketSession
{
	DECLARE_CLASS(RappelzSession)

public:
	virtual void onPacketReceived(const TS_MESSAGE* packet);

	void sendPacket(const TS_MESSAGE* data) { getSocket()->sendPacket(data); }
	void abortSession() { getSocket()->abort(); }

protected:
	RappelzSession(EncryptedSocket::Mode mode);  //instance create by factory method

	void addPacketsToListen(int packetsIdNum, int firstPacketId, ...);
	RappelzSocket* getSocket() { return static_cast<RappelzSocket*>(SocketSession::getSocket()); }

	static void onDataReceived(IListener* instance, RappelzSocket* clientSocket, const TS_MESSAGE* packet);
};

#endif // RAPPELZSESSION_H
