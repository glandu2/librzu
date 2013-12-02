#ifndef ISOCKET_H
#define ISOCKET_H

#include "IObject.h"
#include <string>
#include <stdint.h>
#include "ICallbackGuard.h"

class ISocketListener;

class ISocket : public IObject
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

	typedef void (*CallbackOnDataReady)(void* instance, ISocket* socket);
	typedef void (*CallbackOnStateChanged)(void* instance, ISocket* socket, State oldState, State newState);
	typedef void (*CallbackOnError)(void* instance, ISocket* socket, int errnoValue);

public:
	virtual void IFACECALLCONV deleteLater() = 0;

	virtual bool IFACECALLCONV connect(const std::string & hostName, uint16_t port) = 0;
	virtual bool IFACECALLCONV listen(const std::string& interfaceIp, uint16_t port) = 0;

	virtual size_t IFACECALLCONV read(void *buffer, size_t size) = 0;
	virtual size_t IFACECALLCONV write(const void *buffer, size_t size) = 0;
	virtual bool IFACECALLCONV accept(ISocket *socket) = 0;

	virtual void IFACECALLCONV close() = 0;
	virtual void IFACECALLCONV abort() = 0;
	virtual State IFACECALLCONV getState() = 0;
	virtual size_t IFACECALLCONV getAvailableBytes() = 0;

	virtual int64_t IFACECALLCONV getFd() = 0;

	virtual DelegateRef IFACECALLCONV addDataListener(void* instance, CallbackOnDataReady listener) = 0;
	virtual DelegateRef IFACECALLCONV addConnectionListener(void* instance, CallbackOnDataReady listener) = 0;
	virtual DelegateRef IFACECALLCONV addEventListener(void* instance, CallbackOnStateChanged listener) = 0;
	virtual DelegateRef IFACECALLCONV addErrorListener(void* instance, CallbackOnError listener) = 0;
	virtual void IFACECALLCONV removeListener(void* instance) = 0;
};

#endif // ISOCKET_H
