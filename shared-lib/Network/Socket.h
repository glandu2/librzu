#ifndef SOCKET_H
#define SOCKET_H

#include "../Interfaces/ICallbackGuard.h"
#include "stdint.h"

#include <string>

struct SocketInternal;
class SocketPoll;

class Socket
{
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
	Socket();
	virtual ~Socket();

	void deleteLater();

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

	int64_t getFd();
	unsigned int getLastError();

	void notifyReadyRead();
	void notifyReadyWrite();
	void notifyReadyError(int errorValue);

	static void setPoll(SocketPoll* socketPoll);

protected:
	void setState(State state);

private:
	SocketInternal *_p;
	int lastError;
	static SocketPoll* socketPoll;
	std::string host;
	uint16_t port;
};

#endif // SOCKET_H
