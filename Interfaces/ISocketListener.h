#ifndef ISOCKETLISTENER_H
#define ISOCKETLISTENER_H

#include "IObject.h"
#include "ISocket.h"

class ISocketListener : public IObject
{
public:
	virtual void IFACECALLCONV onDataReceived(ISocket *socket) {}
	virtual void IFACECALLCONV onStateChanged(ISocket *socket, ISocket::State oldState, ISocket::State newState) {}
	virtual void IFACECALLCONV onError(ISocket *socket, int errorno) {}
};

#endif // ISOCKETLISTENER_H
