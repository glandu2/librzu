#ifndef RAPPELZSERVER_H
#define RAPPELZSERVER_H

#include "Object.h"
#include "Socket.h"
#include <list>
#include "uv.h"

class SocketSession;
class BanManager;
class Log;

class RAPPELZLIB_EXTERN RappelzServerCommon : public Object, public IListener
{
	DECLARE_CLASS(RappelzServerCommon)

public:
	//Timeout idle connection (real timeout vary between idleTimeoutSec and idleTimeoutSec*2 seconds)
	RappelzServerCommon(cval<int>* idleTimeoutSec = nullptr, Log* packetLogger = nullptr);
	~RappelzServerCommon();

	bool startServer(const std::string& interfaceIp, uint16_t port, BanManager* banManager = nullptr);
	void stop();
	bool isListening() { return serverSocket->getState() == Stream::ListeningState; }

	Stream::State getState() { return serverSocket->getState(); }

	void socketClosed(std::list<Stream*>::iterator socketIterator) { if(openServer) sockets.erase(socketIterator); }

protected:
	static void onNewConnection(IListener* instance, Stream* serverSocket);
	static void onSocketStateChanged(IListener* instance, Stream*, Stream::State, Stream::State newState);
	static void onCheckIdleSockets(uv_timer_t* timer);

	virtual SocketSession* createSession() = 0;

private:
	bool openServer;
	Stream* serverSocket;
	SocketSession* lastWaitingInstance;
	std::list<Stream*> sockets;
	BanManager* banManager;
	Log* packetLogger;
	uv_timer_t checkIdleSocketTimer;
	cval<int>* checkIdleSocketPeriod;
};

template<class T>
class RappelzServer : public RappelzServerCommon
{
public:
	RappelzServer(cval<int>* idleTimeoutSec = nullptr, Log* packetLogger = nullptr) : RappelzServerCommon(idleTimeoutSec, packetLogger) {}

	void updateObjectName() {
		setObjectName(15 + T::getStaticClassNameSize(), "RappelzServer<%s>", T::getStaticClassName());
	}

protected:
	virtual SocketSession* createSession() {
		return new T();
	}
};

#endif // RAPPELZSERVER_H
