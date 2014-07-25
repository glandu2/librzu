#include "RappelzServer.h"
#include "EventLoop.h"
#include "SocketSession.h"
#include "BanManager.h"
#include "RappelzLibConfig.h"

RappelzServerCommon::RappelzServerCommon(int idleTimeoutSec, Log *packetLogger)
	: openServer(false),
	  serverSocket(new Socket(EventLoop::getLoop())),
	  lastWaitingInstance(nullptr),
	  banManager(nullptr),
	  packetLogger(packetLogger),
	  checkIdleSocketPeriod(idleTimeoutSec)
{
	serverSocket->addConnectionListener(this, &onNewConnection);
	uv_timer_init(EventLoop::getLoop(), &checkIdleSocketTimer);
	checkIdleSocketTimer.data = this;
}

RappelzServerCommon::~RappelzServerCommon() {
	stop();

	if(lastWaitingInstance)
		delete lastWaitingInstance;

	serverSocket->deleteLater();
}

bool RappelzServerCommon::startServer(const std::string &interfaceIp, uint16_t port, BanManager *banManager) {
	this->banManager = banManager;
	openServer = true;
	if(checkIdleSocketPeriod) {
		uint64_t timeoutMs = uint64_t(checkIdleSocketPeriod)*1000;
		uv_timer_start(&checkIdleSocketTimer, &onCheckIdleSockets, timeoutMs, timeoutMs);
	}
	return serverSocket->listen(interfaceIp, port);
}

void RappelzServerCommon::stop() {
	if(checkIdleSocketPeriod)
		uv_timer_stop(&checkIdleSocketTimer);
	serverSocket->close();
	openServer = false;
	for(auto it = sockets.begin(); it != sockets.end();) {
		(*it)->close();
		it = sockets.erase(it);
	}
}

void RappelzServerCommon::onNewConnection(IListener* instance, Socket* serverSocket) {
	RappelzServerCommon* thisInstance = static_cast<RappelzServerCommon*>(instance);

	if(!thisInstance->openServer)
		return;

	if(thisInstance->lastWaitingInstance == nullptr) {
		thisInstance->lastWaitingInstance = thisInstance->createSession();
		thisInstance->lastWaitingInstance->socket->setPacketLogger(thisInstance->packetLogger);
		thisInstance->lastWaitingInstance->socket->addEventListener(thisInstance->lastWaitingInstance, &onSocketStateChanged);
	}

	if(serverSocket->accept(thisInstance->lastWaitingInstance->getSocket())) {
		if(thisInstance->banManager && thisInstance->banManager->isBanned(thisInstance->lastWaitingInstance->getSocket()->getRemoteHost())) {
			thisInstance->lastWaitingInstance->getSocket()->abort();
			thisInstance->debug("Kick banned ip %s\n", thisInstance->lastWaitingInstance->getSocket()->getRemoteHostName());
		} else {
			thisInstance->sockets.push_back(thisInstance->lastWaitingInstance->getSocket());
			thisInstance->lastWaitingInstance->setServer(thisInstance, --thisInstance->sockets.end());
		}
		CONFIG_GET()->stats.connectionCount++;

		thisInstance->lastWaitingInstance->onConnected();

		thisInstance->lastWaitingInstance = nullptr;
	}
}

void RappelzServerCommon::onSocketStateChanged(IListener* instance, Socket*, Socket::State, Socket::State newState) {
	SocketSession* thisInstance = static_cast<SocketSession*>(instance);

	if(newState == Socket::UnconnectedState) {
		CONFIG_GET()->stats.disconnectionCount++;
		RappelzServerCommon* server = thisInstance->getServer();
		if(server)
			server->socketClosed(thisInstance->getSocketIterator());
		delete thisInstance;
	}
}

void RappelzServerCommon::onCheckIdleSockets(uv_timer_t* timer) {
	RappelzServerCommon* thisInstance = static_cast<RappelzServerCommon*>(timer->data);
	thisInstance->trace("Idle socket check\n");
	for(auto it = thisInstance->sockets.begin(); it != thisInstance->sockets.end(); ++it) {
		Socket* socket = *it;
		if(socket->getState() == Socket::ConnectedState && socket->isPacketTransferedSinceLastCheck() == false) {
			socket->close();
			thisInstance->info("Kicked idle connection: %s:%d\n", socket->getRemoteHostName(), socket->getRemotePort());
		} else {
			socket->resetPacketTransferedFlag();
		}
	}
}
