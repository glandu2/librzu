#ifndef SESSIONSERVER_H
#define SESSIONSERVER_H

#include "Object.h"
#include "Socket.h"
#include <list>
#include "uv.h"

class SocketSession;
class BanManager;
class Log;

class LIB_EXTERN SessionServerCommon : public Object, public IListener
{
	DECLARE_CLASS(SessionServerCommon)

public:
	//Timeout idle connection (real timeout vary between idleTimeoutSec and idleTimeoutSec*2 seconds)
	SessionServerCommon(cval<int>* idleTimeoutSec = nullptr, Log* packetLogger = nullptr);
	~SessionServerCommon();

	bool startServer(const std::string& interfaceIp, uint16_t port, BanManager* banManager = nullptr);
	void stop();
	bool isListening() { return serverSocket && serverSocket->getState() == Stream::ListeningState; }

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
	cval<int>* checkIdleSocketPeriod;
};

template<class T>
class SessionServer : public SessionServerCommon
{
public:
	SessionServer(cval<int>* idleTimeoutSec = nullptr, Log* packetLogger = nullptr) : SessionServerCommon(idleTimeoutSec, packetLogger) {}

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
