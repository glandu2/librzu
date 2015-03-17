#ifndef SOCKETSESSION_H
#define SOCKETSESSION_H

#include "Object.h"
#include "Socket.h"
#include <list>

class SessionServerCommon;
class Log;

class RZU_EXTERN SocketSession : public Object, public IListener
{
	DECLARE_CLASS(SocketSession)
public:
	SocketSession();
	virtual void assignStream(Stream* stream);

	virtual bool connect(const char* url, uint16_t port);
	void close() { stream->close(); }
	void abortSession() { stream->abort(); }
	void closeSession() { stream->close(); }

	Stream* getStream() { return stream; }
	virtual size_t read(void *buffer, size_t size) { return stream->read(buffer, size); }
	virtual size_t write(const void *buffer, size_t size) { return stream->write(buffer, size); }

	virtual void onDataReceived() {}
	virtual void onConnected() {}
	virtual void onDisconnected(bool causedByRemote) {}
	virtual void onStateChanged(Stream::State oldState, Stream::State newState, bool causedByRemote) {}
	virtual void onError(int err) {}

	virtual bool hasCustomPacketLogger() { return false; } //used for packet logging
	static bool hasCustomPacketLoggerStatic() { return false; }


protected:
	virtual ~SocketSession(); //deleted when disconnected by SessionServer

	SessionServerCommon* getServer() { return server; }

	void setServer(SessionServerCommon* server, std::list<Stream*>::iterator socketIterator) { this->server = server; this->socketIterator = socketIterator; }
	std::list<Stream*>::iterator getSocketIterator() { return socketIterator; }
	friend class SessionServerCommon;

private:
	static void onDataReceivedStatic(IListener* instance, Stream *stream);
	static void onSocketStateChanged(IListener* instance, Stream*, Stream::State oldState, Stream::State newState, bool causedByRemote);
	static void socketError(IListener* instance, Stream* socket, int errnoValue);

	template<class T> Stream* createStreate(bool logPackets);

private:
	Stream* stream;
	SessionServerCommon* server;
	std::list<Stream*>::iterator socketIterator;
};

#endif // SOCKETSESSION_H
