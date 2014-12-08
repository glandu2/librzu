#include "RappelzServer.h"
#include "EventLoop.h"
#include "SocketSession.h"
#include "BanManager.h"
#include "RappelzLibConfig.h"
#include "PrintfFormats.h"
#include "Pipe.h"
#include "Socket.h"

RappelzServerCommon::RappelzServerCommon(cval<int>* idleTimeoutSec, Log *packetLogger)
	: openServer(false),
	  serverSocket(nullptr),
	  lastWaitingStreamInstance(nullptr),
	  banManager(nullptr),
	  packetLogger(packetLogger),
	  checkIdleSocketPeriod(idleTimeoutSec)
{
	uv_timer_init(EventLoop::getLoop(), &checkIdleSocketTimer);
	checkIdleSocketTimer.data = this;
}

RappelzServerCommon::~RappelzServerCommon() {
	stop();

	if(lastWaitingStreamInstance)
		delete lastWaitingStreamInstance;

	if(serverSocket)
		serverSocket->deleteLater();
}

bool RappelzServerCommon::startServer(const std::string &interfaceIp, uint16_t port, BanManager *banManager) {
	this->banManager = banManager;
	openServer = true;
	int idleTimeout = checkIdleSocketPeriod? checkIdleSocketPeriod->get() : 0;
	if(idleTimeout) {
		uint64_t timeoutMs = uint64_t(idleTimeout)*1000;
		uv_timer_start(&checkIdleSocketTimer, &onCheckIdleSockets, timeoutMs, timeoutMs);
	}

	std::string target;
	int dummy;
	Stream::StreamType type = Stream::parseConnectionUrl(interfaceIp.c_str(), target, dummy);

	switch(type) {
		case Stream::ST_Socket:
			if(!serverSocket || serverSocket->getTrueClassHash() != Socket::getClassHash()) {
				if(serverSocket)
					serverSocket->deleteLater();
				serverSocket = new Socket(EventLoop::getLoop(), false);
				serverSocket->addConnectionListener(this, &onNewConnectionStatic);
			}
			break;

		case Stream::ST_Pipe:
			if(!serverSocket || serverSocket->getTrueClassHash() != Pipe::getClassHash()) {
				if(serverSocket)
					serverSocket->deleteLater();
				serverSocket = new Pipe(EventLoop::getLoop(), false);
				serverSocket->addConnectionListener(this, &onNewConnectionStatic);
			}
			break;
	}

	return serverSocket->listen(target, port);
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

void RappelzServerCommon::onNewConnectionStatic(IListener* instance, Stream* serverSocket) { static_cast<RappelzServerCommon*>(instance)->onNewConnection(); }
void RappelzServerCommon::onNewConnection() {
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

void RappelzServerCommon::onCheckIdleSockets(uv_timer_t* timer) {
	RappelzServerCommon* thisInstance = static_cast<RappelzServerCommon*>(timer->data);
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
