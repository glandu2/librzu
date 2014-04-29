#include "SocketSession.h"
#include "RappelzServer.h"
#include "EventLoop.h"

SocketSession::SocketSession() : socket(new Socket(EventLoop::getLoop())), server(nullptr) {
	socket->addDataListener(this, &SocketSession::onDataReceived);
}

SocketSession::SocketSession(Socket* socket) : socket(socket), server(nullptr) {
	socket->addDataListener(this, &SocketSession::onDataReceived);
}

SocketSession::~SocketSession() {
//	if(server) {
//		server->socketClosed(socketIterator);
//	}
	socket->deleteLater();
}

void SocketSession::onDataReceived(IListener* instance, Socket* socket) {
	SocketSession* thisInstance = static_cast<SocketSession*>(instance);

	thisInstance->onDataReceived();
}
