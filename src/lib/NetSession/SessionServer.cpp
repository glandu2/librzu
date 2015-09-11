#include "SessionServer.h"
#include "Core/EventLoop.h"
#include "SocketSession.h"
#include "Core/PrintfFormats.h"
#include "Stream/Pipe.h"
#include "Stream/Socket.h"
#include "Config/ConfigParamVal.h"
#include "BanManager.h"
#include "Config/GlobalCoreConfig.h"

SessionServerCommon::SessionServerCommon(cval<std::string>& listenIp, cval<int>& port, cval<int>* idleTimeoutSec, Log *packetLogger, BanManager* banManager)
	: openServer(false),
	  serverSocket(nullptr),
	  lastWaitingStreamInstance(nullptr),
	  banManager(banManager),
	  packetLogger(packetLogger),
	  listenIp(listenIp),
	  port(port),
	  checkIdleSocketPeriod(idleTimeoutSec)
{
	uv_timer_init(EventLoop::getLoop(), &checkIdleSocketTimer);
	checkIdleSocketTimer.data = this;
}

SessionServerCommon::~SessionServerCommon() {
	if(serverSocket && serverSocket->getState() != Stream::UnconnectedState)
		stop();
	uv_timer_stop(&checkIdleSocketTimer);

	if(lastWaitingStreamInstance)
		delete lastWaitingStreamInstance;

	if(serverSocket)
		serverSocket->deleteLater();
}

bool SessionServerCommon::start() {
	if(isStarted()) {
		log(LL_Info, "Server %s already started\n", getName());
		return true;
	}

	log(LL_Info, "Starting server %s on %s:%d\n", getName(), listenIp.get().c_str(), port.get());

	if(banManager)
		banManager->loadFile();

	openServer = true;
	int idleTimeout = checkIdleSocketPeriod ? checkIdleSocketPeriod->get() : 0;
	if(idleTimeout) {
		uint64_t timeoutMs = uint64_t(idleTimeout)*1000;
		uv_timer_start(&checkIdleSocketTimer, &onCheckIdleSockets, timeoutMs, timeoutMs);
	}

	std::string target;
	bool streamChanged;
	Stream::StreamType type = Stream::parseConnectionUrl(listenIp.get().c_str(), &target);
	serverSocket = Stream::getStream(type, serverSocket, &streamChanged, !hasCustomPacketLogger());

	if(streamChanged) {
		serverSocket->addConnectionListener(this, &onNewConnectionStatic);
		serverSocket->setPacketLogger(packetLogger);
	}

	return serverSocket->listen(target, port);
}

void SessionServerCommon::stop() {
	if(!serverSocket || serverSocket->getState() == Stream::UnconnectedState) {
		log(LL_Debug, "Server %s already stopped\n", getName());
	}

	log(LL_Info, "Stopping server %s\n", getName());

	uv_timer_stop(&checkIdleSocketTimer);
	serverSocket->close();
	openServer = false;
	for(auto it = sockets.begin(); it != sockets.end();) {
		(*it)->close();
		it = sockets.erase(it);
	}
}

void SessionServerCommon::onNewConnectionStatic(IListener* instance, Stream* serverSocket) { static_cast<SessionServerCommon*>(instance)->onNewConnection(); }
void SessionServerCommon::onNewConnection() {
	if(!openServer)
		return;

	if(serverSocket->accept(&lastWaitingStreamInstance)) {
		if(banManager && banManager->isBanned(lastWaitingStreamInstance->getRemoteIp())) {
			lastWaitingStreamInstance->abort();
			log(LL_Debug, "Kick banned ip %s\n", lastWaitingStreamInstance->getRemoteIpStr());
		} else {
			sockets.push_back(lastWaitingStreamInstance);

			SocketSession* session = createSession();
			session->setServer(this, --sockets.end());
			session->assignStream(lastWaitingStreamInstance);
			session->onConnected();
		}
		CONFIG_GET()->stats.connectionCount++;

		lastWaitingStreamInstance = nullptr;
	}
}

void SessionServerCommon::onCheckIdleSockets(uv_timer_t* timer) {
	SessionServerCommon* thisInstance = static_cast<SessionServerCommon*>(timer->data);
	int kickedConnections = 0;
	uint64_t begin;
	bool logTrace = Log::get() && Log::get()->wouldLog(LL_Trace);

	if(logTrace)
		begin = uv_hrtime();

	for(auto it = thisInstance->sockets.begin(); it != thisInstance->sockets.end();) {
		Stream* socket = *it;
		++it; //if the socket is removed from the list (when closed), we keep a valid iterator
		if(socket->getState() == Stream::ConnectedState) {
			if(socket->isPacketTransferedSinceLastCheck() == false) {
				socket->close();
				kickedConnections++;
				thisInstance->log(LL_Info, "Kicked idle connection: %s:%d\n", socket->getRemoteIpStr(), socket->getRemotePort());
			} else {
				socket->resetPacketTransferedFlag();
			}
		}
	}
	//check fo trace to avoid call to uv_hrtime if not needed
	if(logTrace)
		thisInstance->log(LL_Trace, "Idle socket check: kicked %d sockets in %" PRIu64 " ns\n", kickedConnections, uv_hrtime() - begin);
}
