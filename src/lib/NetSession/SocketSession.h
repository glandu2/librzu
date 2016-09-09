#ifndef SOCKETSESSION_H
#define SOCKETSESSION_H

#include "Core/Object.h"
#include "Core/IListener.h"
#include "Stream/Stream.h"
#include "Core/EventChain.h"
#include <list>

class SessionServerCommon;

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
	virtual size_t write(Stream::WriteRequest* writeRequest) { return stream->write(writeRequest); }
	virtual size_t write(const void *buffer, size_t size) { return stream->write(buffer, size); }
	Stream::State getState() { return stream ? stream->getState() : Stream::UnconnectedState; }

	virtual EventChain<SocketSession> onDataReceived() { return EventChain<SocketSession>(); }
	virtual EventChain<SocketSession> onConnected() { return EventChain<SocketSession>(); }
	virtual EventChain<SocketSession> onDisconnected(bool causedByRemote) { return EventChain<SocketSession>(); }
	virtual EventChain<SocketSession> onStateChanged(Stream::State oldState, Stream::State newState, bool causedByRemote) { return EventChain<SocketSession>(); }
	virtual EventChain<SocketSession> onError(int err) { return EventChain<SocketSession>(); }

	virtual bool hasCustomPacketLogger() { return false; } //used for packet logging
	static bool hasCustomPacketLoggerStatic() { return false; }


protected:
	virtual ~SocketSession(); //deleted when disconnected by SessionServer

	SessionServerCommon* getServer() { return server; }

	void setServer(SessionServerCommon* server);
	friend class SessionServerCommon;

private:
	static void onDataReceivedStatic(IListener* instance, Stream *stream);
	static void onSocketStateChanged(IListener* instance, Stream*, Stream::State oldState, Stream::State newState, bool causedByRemote);
	static void socketError(IListener* instance, Stream* socket, int errnoValue);

private:
	Stream* stream;
	SessionServerCommon* server;
	bool autoDelete;
};

#endif // SOCKETSESSION_H
