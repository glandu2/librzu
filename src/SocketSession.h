#ifndef SOCKETSESSION_H
#define SOCKETSESSION_H

#include "Object.h"
#include "Socket.h"
#include <list>

class RappelzServerCommon;
class Log;

class RAPPELZLIB_EXTERN SocketSession : public Object, public IListener
{
	DECLARE_CLASS(SocketSession)

public:
	SocketSession();
	virtual void assignStream(Stream* stream);

	virtual bool connect(const char* url);
	void abortSession() { stream->abort(); }

	Stream* getStream() { return stream; }
	virtual size_t read(void *buffer, size_t size) { return stream->read(buffer, size); }
	virtual size_t write(const void *buffer, size_t size) { return stream->write(buffer, size); }

	virtual void onDataReceived() {}
	virtual void onConnected() {}
	virtual void onStateChanged(Stream::State oldState, Stream::State newState) {}


protected:
	virtual ~SocketSession(); //deleted when disconnected by RappelzServer

	RappelzServerCommon* getServer() { return server; }

	void setServer(RappelzServerCommon* server, std::list<Stream*>::iterator socketIterator) { this->server = server; this->socketIterator = socketIterator; }
	std::list<Stream*>::iterator getSocketIterator() { return socketIterator; }
	friend class RappelzServerCommon;

private:
	static void onDataReceivedStatic(IListener* instance, Stream *stream);
	static void onSocketStateChanged(IListener* instance, Stream*, Stream::State oldState, Stream::State newState);

	template<class T> Stream* createStreate(bool logPackets);

private:
	Stream* stream;
	RappelzServerCommon* server;
	std::list<Stream*>::iterator socketIterator;
};

#endif // SOCKETSESSION_H
