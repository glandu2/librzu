#ifndef SOCKETPOOL_H
#define SOCKETPOOL_H

#include "ISocketPool.h"
#include <list>

class ISocket;

class SocketPoll : public CImplement<ISocketPool>
{
public:
	SocketPoll();
	~SocketPoll();

	virtual void IFACECALLCONV addSocket(ISocket* socket);
	virtual void IFACECALLCONV removeSocket(ISocket* socket);

	virtual void IFACECALLCONV run();
	virtual void IFACECALLCONV stop();

private:
	int pollAbortPipe[2];
	int epollFd;
	bool stopRequested;
	bool running;
	bool updateAcknownledged;
};

#endif // SOCKETPOOL_H
