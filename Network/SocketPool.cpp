#include "SocketPool.h"
#include "Socket.h"
#include <fcntl.h>
#include <unistd.h>

SocketPool::SocketPool()
{
	stopRequested = false;
	updateAcknownleded = false;
	pipe(pollAbortPipe);
	fcntl(pollAbortPipe[1], F_SETFL, O_NONBLOCK);
	fcntl(pollAbortPipe[0], F_SETFL, O_NONBLOCK);
}

SocketPool::~SocketPool() {
	stopRequested = true;
	updateAcknownleded = false;
	write(pollAbortPipe[1], "c", 1);
	while(!updateAcknownleded)
		usleep(100000);
	close(pollAbortPipe[1]);
	close(pollAbortPipe[0]);
}


void SocketPool::addSocket(ISocket* socket) {
	sockets.push_back(socket);
	write(pollAbortPipe[1], "a", 1);
}

void SocketPool::removeSocket(ISocket* socket) {
	sockets.remove(socket);
	updateAcknownleded = false;
	write(pollAbortPipe[1], "d", 1);
	while(!updateAcknownleded)
		usleep(100000);
}

void SocketPool::run() {
	std::list<ISocket*>::const_iterator it, itEnd;
	int64_t maxfd;
	fd_set readSet;
	int retval;

	while(stopRequested == false) {
		FD_ZERO(&readSet);
		maxfd = -1;
		for(it = sockets.cbegin(), itEnd = sockets.cend(); it != itEnd; ++it) {
			const int fd = (int)(*it)->getFd();
			FD_SET(fd, &readSet);
			if(maxfd < fd)
				maxfd = fd;
		}

		if(pollAbortPipe[0] > maxfd)
			maxfd = pollAbortPipe[0];
		FD_SET(pollAbortPipe[0], &readSet);

		retval = select(maxfd+1, &readSet, NULL, NULL, NULL);
		if(retval < 0) {
			perror("select failed");
		} else {
			for(it = sockets.cbegin(), itEnd = sockets.cend(); it != itEnd; ++it) {
				Socket* socket = static_cast<Socket*>(*it);
				if(FD_ISSET(socket->getFd(), &readSet)) {
					socket->notifyReadyRead();
				}
			}

			if(FD_ISSET(pollAbortPipe[0], &readSet)) {
				char dummy;
				read(pollAbortPipe[0], &dummy, 1);
				updateAcknownleded = true;
			}

		}
	}
}
