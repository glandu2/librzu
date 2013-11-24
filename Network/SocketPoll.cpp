#include "SocketPoll.h"
#include "Socket.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10

SocketPoll::SocketPoll()
{
	stopRequested = false;
	updateAcknownleded = false;
	pipe(pollAbortPipe);
	fcntl(pollAbortPipe[1], F_SETFL, O_NONBLOCK);
	fcntl(pollAbortPipe[0], F_SETFL, O_NONBLOCK);
	epollFd = epoll_create(MAX_EVENTS);
	if(epollFd < 0)
		throw errno;

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.ptr = 0;
	epoll_ctl(epollFd, EPOLL_CTL_ADD, ev.data.fd, &ev);


}

SocketPoll::~SocketPoll() {
	stopRequested = true;
	updateAcknownleded = false;
	write(pollAbortPipe[1], "c", 1);
	while(!updateAcknownleded)
		usleep(100000);
	close(pollAbortPipe[1]);
	close(pollAbortPipe[0]);
	close(epollFd);
}


void SocketPoll::addSocket(ISocket* socket) {
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLET;
	ev.data.ptr = socket;
	epoll_ctl(epollFd, EPOLL_CTL_ADD, ev.data.fd, &ev);

	write(pollAbortPipe[1], "a", 1);
}

void SocketPoll::removeSocket(ISocket* socket) {
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLET;
	ev.data.ptr = socket;
	epoll_ctl(epollFd, EPOLL_CTL_DEL, ev.data.fd, &ev);

	updateAcknownleded = false;
	write(pollAbortPipe[1], "d", 1);
	while(!updateAcknownleded)
		usleep(100000);
}

void SocketPoll::run() {
	int nbev, i;
	struct epoll_event events[MAX_EVENTS];

	while(stopRequested == false) {
		nbev = epoll_wait(epollFd, events, MAX_EVENTS, -1);
		if(nbev < 0) {
			perror("epoll failed");
		} else {
			for(i = 0; i < nbev; i++) {
				if(events[i].data.ptr) {
					Socket* socket = static_cast<Socket*>(events[i].data.ptr);
					if(events[i].events & (EPOLLERR | EPOLLRDHUP)) {
						socket->notifyReadyError();
					} else {
//						if(events[i].events & EPOLLIN)
//							socket->notifyReadyRead();
						if(events[i].events & EPOLLOUT)
							socket->notifyReadyWrite();
					}
				} else {
					int dummy;
					read(pollAbortPipe[0], &dummy, 1);
				}
			}
			updateAcknownleded = true;
		}
	}
}
