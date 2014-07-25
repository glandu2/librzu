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
	RappelzServerCommon(int idleTimeoutSec, Log* packetLogger = nullptr);
	~RappelzServerCommon();

	bool startServer(const std::string& interfaceIp, uint16_t port, BanManager* banManager = nullptr);
	void stop();
	bool isListening() { return serverSocket->getState() == Socket::ListeningState; }

	Socket::State getState() { return serverSocket->getState(); }

	void socketClosed(std::list<Socket*>::iterator socketIterator) { if(openServer) sockets.erase(socketIterator); }

protected:
	static void onNewConnection(IListener* instance, Socket* serverSocket);
	static void onSocketStateChanged(IListener* instance, Socket*, Socket::State, Socket::State newState);
	static void onCheckIdleSockets(uv_timer_t* timer);

	virtual SocketSession* createSession() = 0;

private:
	bool openServer;
	Socket* serverSocket;
	SocketSession* lastWaitingInstance;
	std::list<Socket*> sockets;
	BanManager* banManager;
	Log* packetLogger;
	uv_timer_t checkIdleSocketTimer;
	int checkIdleSocketPeriod;
};

template<class T>
class RappelzServer : public RappelzServerCommon
{
public:
	RappelzServer(int idleTimeoutSec = 0, Log* packetLogger = nullptr) : RappelzServerCommon(idleTimeoutSec, packetLogger) {}

	void updateObjectName() {
		setObjectName(15 + T::getStaticClassNameSize(), "RappelzServer<%s>", T::getStaticClassName());
	}

protected:
	virtual SocketSession* createSession() {
		return new T();
	}
};

#endif // RAPPELZSERVER_H
