#include "SessionServer.h"
#include "EventLoop.h"
#include "SocketSession.h"
#include "BanManager.h"
#include "GlobalCoreConfig.h"
#include "PrintfFormats.h"
#include "Pipe.h"
#include "Socket.h"

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
	stop();

	if(lastWaitingStreamInstance)
		delete lastWaitingStreamInstance;

	if(serverSocket)
		serverSocket->deleteLater();
}

bool SessionServerCommon::start() {
	if(isStarted()) {
		info("Server %s already started\n", getName());
		return true;
	}

	info("Starting server %s on %s:%d\n", getName(), listenIp.get().c_str(), port.get());

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

	if(streamChanged)
		serverSocket->addConnectionListener(this, &onNewConnectionStatic);

	return serverSocket->listen(target, port);
}

void SessionServerCommon::stop() {
	if(!serverSocket || serverSocket->getState() == Stream::UnconnectedState) {
		debug("Server %s already stopped\n", getName());
		return;
	}

	info("Stopping server %s\n", getName());

	if(checkIdleSocketPeriod)
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
		if(banManager && banManager->isBanned(lastWaitingStreamInstance->getRemoteHost())) {
			lastWaitingStreamInstance->abort();
			debug("Kick banned ip %s\n", lastWaitingStreamInstance->getRemoteHostName());
		} else {
			lastWaitingStreamInstance->setPacketLogger(packetLogger);

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
	bool logTrace = Log::get() && Log::get()->wouldLog(Log::LL_Trace);

	if(logTrace)
		begin = uv_hrtime();

	for(auto it = thisInstance->sockets.begin(); it != thisInstance->sockets.end();) {
		Stream* socket = *it;
		++it; //if the socket is removed from the list (when closed), we keep a valid iterator
		if(socket->getState() == Stream::ConnectedState) {
			if(socket->isPacketTransferedSinceLastCheck() == false) {
				socket->close();
				kickedConnections++;
				thisInstance->info("Kicked idle connection: %s:%d\n", socket->getRemoteHostName(), socket->getRemotePort());
			} else {
				socket->resetPacketTransferedFlag();
			}
		}
	}
	//check fo trace to avoid call to uv_hrtime if not needed
	if(logTrace)
		thisInstance->trace("Idle socket check: kicked %d sockets in %" PRIu64 " ns\n", kickedConnections, uv_hrtime() - begin);
}
