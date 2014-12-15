#include "SocketSession.h"
#include "RappelzServer.h"
#include "EventLoop.h"
#include "RappelzLibConfig.h"

#include "Socket.h"
#include "Pipe.h"

SocketSession::SocketSession() : stream(nullptr), server(nullptr) {
}

void SocketSession::assignStream(Stream* stream) {
	this->stream = stream;
	stream->addDataListener(this, &SocketSession::onDataReceivedStatic);
	stream->addEventListener(this, &SocketSession::onSocketStateChanged);
}

bool SocketSession::connect(const char *url, int port) {
	std::string target;
	Stream* newStream;

	Stream::StreamType type = Stream::parseConnectionUrl(url, &target);
	newStream = Stream::getStream(type, stream);

	if(newStream != stream)
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

	thisInstance->onStateChanged(oldState, newState);

	RappelzServerCommon* server = thisInstance->getServer();
	if(newState == Stream::UnconnectedState && server) {
		CONFIG_GET()->stats.disconnectionCount++;
		server->socketClosed(thisInstance->getSocketIterator());
		delete thisInstance;
	}
}
