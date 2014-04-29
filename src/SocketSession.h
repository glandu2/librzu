#ifndef SOCKETSESSION_H
#define SOCKETSESSION_H

#include "Object.h"
#include "Socket.h"
#include <list>

class RappelzServerCommon;

class RAPPELZLIB_EXTERN SocketSession : public Object, public IListener
{
	DECLARE_CLASS(SocketSession)

public:
	virtual void onDataReceived() {}
	virtual void onConnected() {}

protected:
	SocketSession();
	SocketSession(Socket* socket);
	virtual ~SocketSession(); //deleted when disconnected by RappelzServer

	Socket* getSocket() { return socket; }
	RappelzServerCommon* getServer() { return server; }

	static void onDataReceived(IListener* instance, Socket* socket);


	void setServer(RappelzServerCommon* server, std::list<Socket*>::iterator socketIterator) { this->server = server; this->socketIterator = socketIterator; }
	std::list<Socket*>::iterator getSocketIterator() { return socketIterator; }
	friend class RappelzServerCommon;

private:
	Socket* socket;
	RappelzServerCommon* server;
	std::list<Socket*>::iterator socketIterator;
};

#endif // SOCKETSESSION_H
