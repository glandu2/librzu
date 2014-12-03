#include "SocketSession.h"
#include "RappelzServer.h"
#include "EventLoop.h"

SocketSession::SocketSession() : stream(new Socket(EventLoop::getLoop())), server(nullptr) {
	stream->addDataListener(this, &SocketSession::onDataReceived);
}

SocketSession::SocketSession(Stream* stream) : stream(stream), server(nullptr) {
}

SocketSession::~SocketSession() {
	stream->deleteLater();
}

void SocketSession::onDataReceived(IListener* instance, Stream* stream) {
	SocketSession* thisInstance = static_cast<SocketSession*>(instance);

	thisInstance->onDataReceived();
}
