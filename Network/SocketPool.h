#ifndef SOCKETPOOL_H
#define SOCKETPOOL_H

#include "ISocketPool.h"
#include <list>

class ISocket;

class SocketPool : public CImplement<ISocketPool>
{
public:
	SocketPool();
	~SocketPool();

	virtual void IFACECALLCONV addSocket(ISocket* socket);
	virtual void IFACECALLCONV removeSocket(ISocket* socket);

	virtual void IFACECALLCONV run();

private:
	std::list<ISocket*> sockets;
	int pollAbortPipe[2];
	bool stopRequested;
	bool updateAcknownleded;
};

#endif // SOCKETPOOL_H
