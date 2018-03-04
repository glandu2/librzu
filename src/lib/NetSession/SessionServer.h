#ifndef SESSIONSERVER_H
#define SESSIONSERVER_H

#include "Core/Object.h"
#include "Core/Timer.h"
#include "StartableObject.h"
#include "Stream/Stream.h"
#include <list>

class SocketSession;
class BanManager;
class Log;

class RZU_EXTERN SessionServerCommon : public Object, public IListener, public StartableObject {
	DECLARE_CLASS(SessionServerCommon)

public:
	// Timeout idle connection (real timeout vary between idleTimeoutSec and idleTimeoutSec*2 seconds)
	SessionServerCommon(cval<std::string>& listenIp,
	                    cval<int>& port,
	                    cval<int>* idleTimeoutSec = nullptr,
	                    Log* packetLogger = nullptr,
	                    BanManager* banManager = nullptr);
	~SessionServerCommon();

	bool start();
	void stop();
	bool isStarted() { return serverSocket && serverSocket->getState() == Stream::ListeningState; }

	Stream::State getState() { return serverSocket ? serverSocket->getState() : Stream::UnconnectedState; }
	Stream* getServerStream() { return serverSocket; }

	void socketClosed(SocketSession* socketSession);

protected:
	static void onNewConnectionStatic(IListener* instance, Stream* serverSocket);
	void onNewConnection();

	void onCheckIdleSockets();

	virtual SocketSession* createSession() = 0;
	virtual bool hasCustomPacketLogger() = 0;

private:
	bool openServer;
	Stream* serverSocket;
	Stream* lastWaitingStreamInstance;
	std::list<SocketSession*> sockets;
	BanManager* banManager;
	Log* packetLogger;
	Timer<SessionServerCommon> checkIdleSocketTimer;
	cval<std::string>& listenIp;
	cval<int>& port;
	cval<int>* checkIdleSocketPeriod;
};

template<class T> class SessionServer : public SessionServerCommon {
public:
	SessionServer(cval<std::string>& listenIp,
	              cval<int>& port,
	              cval<int>* idleTimeoutSec,
	              Log* packetLogger = nullptr,
	              BanManager* banManager = nullptr)
	    : SessionServerCommon(listenIp, port, idleTimeoutSec, packetLogger, banManager) {}

	void updateObjectName() {
		setObjectName(15 + T::getStaticClassNameSize(), "SessionServer<%s>", T::getStaticClassName());
	}

protected:
	virtual SocketSession* createSession() { return new T(); }
	virtual bool hasCustomPacketLogger() { return T::hasCustomPacketLoggerStatic(); }
};

template<class T, class Param> class SessionServerWithParameter : public SessionServerCommon {
public:
	SessionServerWithParameter(Param parameter,
	                           cval<std::string>& listenIp,
	                           cval<int>& port,
	                           cval<int>* idleTimeoutSec = nullptr,
	                           Log* packetLogger = nullptr,
	                           BanManager* banManager = nullptr)
	    : SessionServerCommon(listenIp, port, idleTimeoutSec, packetLogger, banManager), parameter(parameter) {}

	void updateObjectName() {
		setObjectName(15 + T::getStaticClassNameSize(), "SessionServer<%s>", T::getStaticClassName());
	}

protected:
	virtual SocketSession* createSession() { return new T(parameter); }
	virtual bool hasCustomPacketLogger() { return T::hasCustomPacketLoggerStatic(); }

private:
	Param parameter;
};

#endif  // SESSIONSERVER_H
