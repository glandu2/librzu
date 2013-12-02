#ifndef SOCKETPOOL_H
#define SOCKETPOOL_H

#include <unordered_set>

class Socket;

class SocketPoll
{
public:
	SocketPoll();
	~SocketPoll();

	virtual void addSocket(Socket* socket);
	virtual void removeSocket(Socket* socket);

	virtual void run();
	virtual void processEvents(int waitTime);
	virtual void stop();

	virtual void deleteLater(Socket* socket);

private:
	int pollAbortPipe[2];
	int epollFd;
	bool stopRequested;
	bool running;
	bool updateAcknownledged;
	std::unordered_set<Socket*> socketsToDelete;
};

#endif // SOCKETPOOL_H
