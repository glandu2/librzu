#include "SocketSession.h"
#include "RappelzServer.h"
#include "EventLoop.h"
#include "GlobalCoreConfig.h"

#include "Socket.h"
#include "Pipe.h"

SocketSession::SocketSession() : stream(nullptr), server(nullptr) {
}

void SocketSession::assignStream(Stream* stream) {
	this->stream = stream;
	stream->addDataListener(this, &SocketSession::onDataReceivedStatic);
	stream->addEventListener(this, &SocketSession::onSocketStateChanged);
}

bool SocketSession::connect(const char *url, uint16_t port) {
	std::string target;
	Stream* newStream;
	bool changed;

	Stream::StreamType type = Stream::parseConnectionUrl(url, &target);
	newStream = Stream::getStream(type, stream, &changed, !isEncryptedSession());

	if(changed)
		assignStream(newStream);

	return stream->connect(target, port);
}

SocketSession::~SocketSession() {
	stream->deleteLater();
}

void SocketSession::onDataReceivedStatic(IListener* instance, Stream* stream) {
	SocketSession* thisInstance = static_cast<SocketSession*>(instance);

	thisInstance->onDataReceived();
}

void SocketSession::onSocketStateChanged(IListener* instance, Stream*, Stream::State oldState, Stream::State newState) {
	SocketSession* thisInstance = static_cast<SocketSession*>(instance);

	if(newState == Stream::ConnectedState)
		thisInstance->onConnected();

	thisInstance->onStateChanged(oldState, newState);

	if(newState == Stream::UnconnectedState)
		thisInstance->onDisconnected();

	RappelzServerCommon* server = thisInstance->getServer();
	if(newState == Stream::UnconnectedState && server) {
		CONFIG_GET()->stats.disconnectionCount++;
		server->socketClosed(thisInstance->getSocketIterator());
		delete thisInstance;
	}
}

void SocketSession::socketError(IListener *instance, Stream*, int errnoValue) {
	SocketSession* thisInstance = static_cast<SocketSession*>(instance);

	thisInstance->onError(errnoValue);
}
