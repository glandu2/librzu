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

bool SocketSession::connect(const char *url) {
	std::string target;
	int port;
	Stream::StreamType type = Stream::parseConnectionUrl(url, target, port);
	Stream* newStream = nullptr;

	switch(type) {
		case Stream::ST_Socket:
			if(!stream || stream->getTrueClassHash() != Socket::getClassHash()) {
				if(stream)
					stream->deleteLater();
				newStream = new Socket(EventLoop::getLoop(), false);
			}
			break;

		case Stream::ST_Pipe:
			if(!stream || stream->getTrueClassHash() != Pipe::getClassHash()) {
				if(stream)
					stream->deleteLater();
				newStream = new Pipe(EventLoop::getLoop(), false);
			}
			break;
	}

	if(newStream)
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
