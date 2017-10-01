#include "SocketSession.h"
#include "Config/GlobalCoreConfig.h"
#include "SessionServer.h"

#include "Stream/Pipe.h"
#include "Stream/Socket.h"

SocketSession::SocketSession() : stream(nullptr), server(nullptr), autoDelete(false) {}

void SocketSession::assignStream(Stream* stream) {
	this->stream = stream;
	stream->addDataListener(this, &SocketSession::onDataReceivedStatic);
	stream->addEventListener(this, &SocketSession::onSocketStateChanged);
}

bool SocketSession::connect(const char* url, uint16_t port) {
	std::string target;
	Stream* newStream;
	bool changed;

	Stream::StreamType type = Stream::parseConnectionUrl(url, &target);
	newStream = Stream::getStream(type, stream, &changed, !hasCustomPacketLogger());

	if(changed)
		assignStream(newStream);

	return stream->connect(target, port);
}

SocketSession::~SocketSession() {
	if(stream)
		stream->deleteLater();
}

void SocketSession::setServer(SessionServerCommon* server) {
	this->server = server;
	this->autoDelete = true;
}

void SocketSession::onDataReceivedStatic(IListener* instance, Stream* stream) {
	SocketSession* thisInstance = static_cast<SocketSession*>(instance);

	thisInstance->onDataReceived();
}

void SocketSession::onSocketStateChanged(
    IListener* instance, Stream*, Stream::State oldState, Stream::State newState, bool causedByRemote) {
	SocketSession* thisInstance = static_cast<SocketSession*>(instance);

	if(newState == Stream::ConnectedState)
		thisInstance->onConnected();

	thisInstance->onStateChanged(oldState, newState, causedByRemote);

	if(newState == Stream::UnconnectedState)
		thisInstance->onDisconnected(causedByRemote);

	if(newState == Stream::UnconnectedState && thisInstance->autoDelete) {
		GlobalCoreConfig::get()->stats.disconnectionCount++;

		SessionServerCommon* server = thisInstance->getServer();
		if(server)
			server->socketClosed(thisInstance);

		if(thisInstance->isScheduledForDelete() == false)
			delete thisInstance;
	}
}

void SocketSession::socketError(IListener* instance, Stream*, int errnoValue) {
	SocketSession* thisInstance = static_cast<SocketSession*>(instance);

	thisInstance->onError(errnoValue);
}
