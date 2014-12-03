#include "SocketSession.h"
#include "RappelzServer.h"
#include "EventLoop.h"

#include "Socket.h"
#include "Pipe.h"

SocketSession::SocketSession() : stream(nullptr), server(nullptr) {
}

void SocketSession::assignStream(Stream* stream) {
	this->stream = stream;
	stream->addDataListener(this, &SocketSession::onDataReceived);
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

void SocketSession::onDataReceived(IListener* instance, Stream* stream) {
	SocketSession* thisInstance = static_cast<SocketSession*>(instance);

	thisInstance->onDataReceived();
}
