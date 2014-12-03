#ifndef SOCKETSESSION_H
#define SOCKETSESSION_H

#include "Object.h"
#include "Socket.h"
#include <list>

class RappelzServerCommon;
class Log;

class RAPPELZLIB_EXTERN SocketSession : public Object, public IListener
{
	DECLARE_CLASS(SocketSession)

public:
	virtual void onDataReceived() {}
	virtual void onConnected() {}

protected:
	SocketSession();
	SocketSession(Stream* socket);
	virtual ~SocketSession(); //deleted when disconnected by RappelzServer

	Stream* getSocket() { return stream; }
	RappelzServerCommon* getServer() { return server; }

	void setServer(RappelzServerCommon* server, std::list<Stream*>::iterator socketIterator) { this->server = server; this->socketIterator = socketIterator; }
	std::list<Stream*>::iterator getSocketIterator() { return socketIterator; }
	friend class RappelzServerCommon;

private:
	static void onDataReceived(IListener* instance, Stream *stream);

private:
	Stream* stream;
	RappelzServerCommon* server;
	std::list<Stream*>::iterator socketIterator;
};

#endif // SOCKETSESSION_H
