#include "SocketPoll.h"
#include "Socket.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#define MAX_EVENTS 10

SocketPoll::SocketPoll()
{
	stopRequested = false;
	running = false;
	updateAcknownledged = false;
	pipe(pollAbortPipe);
	fcntl(pollAbortPipe[1], F_SETFL, O_NONBLOCK);
	fcntl(pollAbortPipe[0], F_SETFL, O_NONBLOCK);
	epollFd = epoll_create(MAX_EVENTS);
	if(epollFd < 0)
		throw errno;

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.ptr = 0;
	epoll_ctl(epollFd, EPOLL_CTL_ADD, pollAbortPipe[0], &ev);

	Socket::setPoll(this);
}

SocketPoll::~SocketPoll() {
	if(running) {
		stopRequested = true;
		updateAcknownledged = false;
		write(pollAbortPipe[1], "c", 1);
		while(!updateAcknownledged)
			usleep(100000);
	}
	close(pollAbortPipe[1]);
	close(pollAbortPipe[0]);
	close(epollFd);
}


void SocketPoll::addSocket(ISocket* socket) {
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLERR| EPOLLOUT | EPOLLRDHUP | EPOLLET;
	ev.data.ptr = socket;
	epoll_ctl(epollFd, EPOLL_CTL_ADD, socket->getFd(), &ev);

	write(pollAbortPipe[1], "a", 1);
}

void SocketPoll::removeSocket(ISocket* socket) {
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLERR | EPOLLOUT | EPOLLRDHUP | EPOLLET;
	ev.data.ptr = socket;
	epoll_ctl(epollFd, EPOLL_CTL_DEL,socket->getFd(), &ev);

	write(pollAbortPipe[1], "d", 1);
}

void SocketPoll::run() {
	running = true;
	while(stopRequested == false) {
		processEvents(-1);
		updateAcknownledged = true;
	}
	running = false;
}

void SocketPoll::processEvents(int waitTime) {
	int nbev, i;
	struct epoll_event events[MAX_EVENTS];

	nbev = epoll_wait(epollFd, events, MAX_EVENTS, waitTime);
	if(nbev < 0) {
		perror("epoll failed");
	} else {
		for(i = 0; i < nbev; i++) {
			fprintf(stderr, "EPOLL %p %x\n", events[i].data.ptr, events[i].events);
			if(events[i].data.ptr) {
				if(events[i].events & EPOLLHUP)
					continue;

				Socket* socket = static_cast<Socket*>(events[i].data.ptr);
				if(events[i].events & (EPOLLERR | EPOLLRDHUP)) {
					int socketError;
					unsigned int errorSize = sizeof(int);
					::getsockopt(socket->getFd(), SOL_SOCKET, SO_ERROR, &socketError, &errorSize);
					socket->notifyReadyError(socketError);
				} else {
					if(events[i].events & EPOLLIN) {
						socket->notifyReadyRead();
					}
					if(events[i].events & EPOLLOUT) {
						socket->notifyReadyWrite();
					}
				}
			} else {
				int dummy;
				if(read(pollAbortPipe[0], &dummy, 1) > 0)
					fprintf(stderr, "epoll changed, type=%c\n", dummy & 0xFF);
			}
		}
	}
}

void SocketPoll::stop() {
	stopRequested = true;
	write(pollAbortPipe[1], "s", 1);
}
