#ifndef SOCKET_H
#define SOCKET_H

#include "Object.h"
#include "uv.h"
#include "ICallbackGuard.h"
#include "stdint.h"

#include <string>

struct SocketInternal;
class SocketPoll;

class RAPPELZLIB_EXTERN Socket : public Object
{
	DECLARE_CLASS(Socket)

public:
	enum State {
		UnconnectedState,	//Client & server

		ConnectingState,	//Client
		Binding,			//Server

		Listening,			//Server
		ConnectedState,		//Client

		ClosingState		//Client & Server
	};

	typedef void (*CallbackOnDataReady)(void* instance, Socket* socket);
	typedef void (*CallbackOnStateChanged)(void* instance, Socket* socket, State oldState, State newState);
	typedef void (*CallbackOnError)(void* instance, Socket* socket, int errnoValue);

public:
	Socket(uv_loop_t* uvLoop);
	virtual ~Socket();

	virtual bool connect(const std::string& hostName, uint16_t port);
	virtual bool listen(const std::string& interfaceIp, uint16_t port);

	virtual size_t getAvailableBytes();
	virtual size_t read(void *buffer, size_t size);
	virtual size_t write(const void *buffer, size_t size);
	virtual bool accept(Socket *socket);

	virtual void close();
	virtual void abort();

	State getState();
	const std::string& getHost() { return host; }
	uint16_t getPort() { return port; }

	DelegateRef addDataListener(void* instance, CallbackOnDataReady listener);
	DelegateRef addConnectionListener(void* instance, CallbackOnDataReady listener);
	DelegateRef addEventListener(void* instance, CallbackOnStateChanged listener);
	DelegateRef addErrorListener(void* instance, CallbackOnError listener);
	void removeListener(void* instance);

	void notifyReadyError(int errorValue);


protected:
	void setState(State state);
	static void onConnected(uv_connect_t* req, int status);
	static void onNewConnection(uv_stream_t* req, int status);

	static void onAllocReceiveBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
	static void onReadCompleted(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void onWriteCompleted(uv_write_t* req, int status);

	static void onShutdownDone(uv_shutdown_t* req, int status);
	static void onConnectionClosed(uv_handle_t* handle);

private:
	SocketInternal *_p;
	int lastError;
	uv_loop_t* uvLoop;
	std::string host;
	uint16_t port;
};

#endif // SOCKET_H
