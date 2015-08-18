#ifndef SESSIONSERVER_H
#define SESSIONSERVER_H

#include "Object.h"
#include "StartableObject.h"
#include "Stream.h"
#include <list>
#include "uv.h"

class SocketSession;
class BanManager;
class Log;

class RZU_EXTERN SessionServerCommon : public Object, public IListener, public StartableObject
{
	DECLARE_CLASS(SessionServerCommon)

public:
	//Timeout idle connection (real timeout vary between idleTimeoutSec and idleTimeoutSec*2 seconds)
	SessionServerCommon(cval<std::string>& listenIp, cval<int>& port, cval<int>* idleTimeoutSec = nullptr, Log* packetLogger = nullptr, BanManager* banManager = nullptr);
	~SessionServerCommon();

	bool start();
	void stop();
	bool isStarted() { return serverSocket && serverSocket->getState() == Stream::ListeningState; }

	Stream::State getState() { return serverSocket ? serverSocket->getState() : Stream::UnconnectedState; }

	void socketClosed(std::list<Stream*>::iterator socketIterator) { if(openServer) sockets.erase(socketIterator); }

protected:
	static void onNewConnectionStatic(IListener* instance, Stream *serverSocket);
	void onNewConnection();

	static void onCheckIdleSockets(uv_timer_t* timer);

	virtual SocketSession* createSession() = 0;
	virtual bool hasCustomPacketLogger() = 0;

private:
	bool openServer;
	Stream* serverSocket;
	Stream* lastWaitingStreamInstance;
	std::list<Stream*> sockets;
	BanManager* banManager;
	Log* packetLogger;
	uv_timer_t checkIdleSocketTimer;
	cval<std::string>& listenIp;
	cval<int>& port;
	cval<int>* checkIdleSocketPeriod;
};

template<class T>
class SessionServer : public SessionServerCommon
{
public:
	SessionServer(cval<std::string>& listenIp, cval<int>& port, cval<int>* idleTimeoutSec = nullptr, Log* packetLogger = nullptr, BanManager* banManager = nullptr) : SessionServerCommon(listenIp, port, idleTimeoutSec, packetLogger, banManager) {}

	void updateObjectName() {
		setObjectName(15 + T::getStaticClassNameSize(), "SessionServer<%s>", T::getStaticClassName());
	}

protected:
	virtual SocketSession* createSession() {
		return new T();
	}
	virtual bool hasCustomPacketLogger() {
		return T::hasCustomPacketLoggerStatic();
	}
};

#endif // SESSIONSERVER_H
