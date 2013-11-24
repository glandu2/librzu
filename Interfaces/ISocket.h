#ifndef ISOCKET_H
#define ISOCKET_H

#include "IObject.h"
#include <string>
#include <stdint.h>

class ISocketListener;

class ISocket : public IObject
{
public:
	enum State {
		UnconnectedState,
		HostLookupState,
		ConnectingState,
		ConnectedState,
		ClosingState
	};

	typedef void (*CallbackOnDataReceived)(void* instance, ISocket* socket);
	typedef void (*CallbackOnStateChanged)(void* instance, ISocket* socket, State oldState, State newState);
	typedef void (*CallbackOnError)(void* instance, ISocket* socket, int errno);

public:
	virtual bool IFACECALLCONV connect(const std::string & hostName, uint16_t port) = 0;
	virtual size_t IFACECALLCONV read(void *buffer, size_t size) = 0;
	virtual size_t IFACECALLCONV write(const void *buffer, size_t size) = 0;
	virtual void IFACECALLCONV close() = 0;
	virtual void IFACECALLCONV abort() = 0;
	virtual void IFACECALLCONV getState(State state) = 0;
	virtual size_t IFACECALLCONV getAvailableBytes() = 0;

	virtual int64_t IFACECALLCONV getFd() = 0;

	virtual void IFACECALLCONV addDataListener(void* instance, CallbackOnDataReceived listener) = 0;
	virtual void IFACECALLCONV addEventListener(void* instance, CallbackOnStateChanged listener) = 0;
	virtual void IFACECALLCONV addErrorListener(void* instance, CallbackOnError listener) = 0;
	virtual void IFACECALLCONV removeListener(void* instance) = 0;
};

#endif // ISOCKET_H
