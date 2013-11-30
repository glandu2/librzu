#include "Socket.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include "SocketPoll.h"

#include <sys/ioctl.h>

#include <unordered_map>
#include "../Common/Delegate.h"

#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif

struct SocketInternal {
	std::unordered_map<void*, Socket::CallbackOnDataReady> dataListeners;
	std::unordered_map<void*, Socket::CallbackOnStateChanged> eventListeners;
	std::unordered_map<void*, Socket::CallbackOnError> errorListeners;
	int sock;
	Socket::State currentState;
};

SocketPoll* Socket::socketPoll = 0;

Socket::Socket() : _p(new SocketInternal)
{
	_p->sock = -1;
	_p->currentState = UnconnectedState;
	lastError = 0;
}

Socket::~Socket() {
	if(getState() != UnconnectedState)
		abort();
	delete _p;
}

bool Socket::connect(const std::string & hostName, uint16_t port) {
	if(getState() != UnconnectedState)
		return false;

	if(_p->sock >= 0)
		abort();

	_p->sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if(_p->sock < 0) {
		this->lastError = errno;
		notifyReadyError(lastError);
		return false;
	}

	setState(ConnectingState);

	int optValue = 1;
	int optionSize = sizeof(int);
	::setsockopt(_p->sock, SOL_SOCKET, SO_OOBINLINE, &optValue, optionSize);
	optValue = 1;
	::ioctl(_p->sock, FIONBIO, (char *)&optValue);

	if(socketPoll)
		socketPoll->addSocket(this);

	struct sockaddr_in hostAddress;
	struct addrinfo hints, *servinfo, *p;
    int rv, lastError;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;

	if( (rv = getaddrinfo( hostName.c_str() , NULL, &hints , &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		hostAddress = *(struct sockaddr_in*)p->ai_addr;
		hostAddress.sin_port = htons(port);
		lastError = ::connect(_p->sock, (sockaddr*)&hostAddress, sizeof(hostAddress));
		if(lastError < 0)
			lastError = -errno;
		if(lastError == 0 || lastError == -EINPROGRESS)
			break;
	}

	if(lastError != 0 && lastError != -EINPROGRESS) {
		this->lastError = errno;
		notifyReadyError(lastError);
		return false;
	} else if(lastError == 0) {
		setState(ConnectedState);
	}
	return true;
}

size_t Socket::read(void *buffer, size_t size) {
	size_t totalByteRead;
	int byteRead;

	for(totalByteRead = 0; totalByteRead < size;) {
		byteRead = ::recv(_p->sock, static_cast<char*>(buffer) + totalByteRead, size - totalByteRead, 0);
		if(byteRead < 0) {
			break;
		}
		totalByteRead += byteRead;
	}

	if(byteRead < 0) {
		if(errno != EAGAIN) {
			lastError = errno;
			notifyReadyError(lastError);
		}
	}

	return totalByteRead;
}

size_t Socket::write(const void *buffer, size_t size) {
	size_t totalByteWritten;
	int byteWritten;

	for(totalByteWritten = 0; totalByteWritten < size;) {
		byteWritten = ::send(_p->sock, static_cast<const char*>(buffer) + totalByteWritten, size - totalByteWritten, 0);
		if(byteWritten < 0) {
			if(errno == EAGAIN) {
				fd_set readySet;
				FD_ZERO(&readySet);
				FD_SET(_p->sock, &readySet);
				select(_p->sock+1, NULL, &readySet, NULL, NULL);
				continue;
			}
			else
				break;
		}
		totalByteWritten += byteWritten;
	}

	if(byteWritten < 0) {
		lastError = errno;
		notifyReadyError(lastError);
	}

	return totalByteWritten;
}

void Socket::close() {
	if(_p->sock < 0)
		return;

	if(socketPoll)
		socketPoll->removeSocket(this);

	setState(ClosingState);
	::shutdown(_p->sock, SHUT_RDWR);
	::close(_p->sock);
	_p->sock = -1;
	setState(UnconnectedState);
}

void Socket::abort() {
	if(_p->sock < 0)
		return;

	int lingerValue = 0;
	int lingerOptionSize = sizeof(int);
	::setsockopt(_p->sock, SOL_SOCKET, SO_LINGER, &lingerValue, lingerOptionSize);
	close();
}

size_t Socket::getAvailableBytes() {
	int nbytes = 0;

	if(ioctl(_p->sock, FIONREAD, (char *) &nbytes) >= 0)
		return nbytes;

	return 0;
}

Socket::State Socket::getState() {
	return _p->currentState;
}

void Socket::setState(State state) {
	if(state == _p->currentState)
		return;


	State oldState = _p->currentState;

	_p->currentState = state;

	fprintf(stderr, "Socket state change from %d to %d : %d\n", oldState, state, _p->sock);

	std::unordered_map<void*, CallbackOnStateChanged>::const_iterator it, itEnd;
	for(it = _p->eventListeners.cbegin(), itEnd = _p->eventListeners.cend(); it != itEnd; ++it) {
		void* instance = it->first;
		const CallbackOnStateChanged& callback = it->second;

		callback(instance, this, oldState, state);
	}
}

unsigned int Socket::getLastError() {
	return lastError;
}

ICallbackGuard::CallbackPtr Socket::addDataListener(void* instance, CallbackOnDataReady listener) {
	std::unordered_map<void*, Socket::CallbackOnDataReady>::iterator it;

	it = _p->dataListeners.emplace(instance, listener).first;

	return (ICallbackGuard::CallbackPtr)(&(it->second));
}

ICallbackGuard::CallbackPtr Socket::addEventListener(void* instance, CallbackOnStateChanged listener) {
	std::unordered_map<void*, Socket::CallbackOnStateChanged>::iterator it;

	it = _p->eventListeners.emplace(instance, listener).first;

	return (ICallbackGuard::CallbackPtr)(&(it->second));
}

ICallbackGuard::CallbackPtr Socket::addErrorListener(void* instance, CallbackOnError listener) {
	std::unordered_map<void*, Socket::CallbackOnError>::iterator it;

	it = _p->errorListeners.emplace(instance, listener).first;

	return (ICallbackGuard::CallbackPtr)(&(it->second));
}

void Socket::removeListener(void* instance) {
	_p->dataListeners.erase(instance);
	_p->eventListeners.erase(instance);
	_p->errorListeners.erase(instance);
}

void Socket::notifyReadyRead() {
	std::unordered_map<void*, CallbackOnDataReady>::const_iterator it, itEnd;

	switch(getState()) {
		case UnconnectedState:
			fprintf(stderr, "Received READ event on closed socket\n");
			break;

		case HostLookupState:
			fprintf(stderr, "Received READ event on host lookup socket\n");
			break;

		case ConnectingState:
			setState(ConnectedState);
			break;

		case ConnectedState:
			for(it = _p->dataListeners.cbegin(), itEnd = _p->dataListeners.cend(); it != itEnd;) {
				void* instance = it->first;
				const CallbackOnDataReady& callback = it->second;

				if(callback != nullptr) {
					callback(instance, this);
					++it;
				} else {
					it = _p->dataListeners.erase(it);
				}
			}
			break;

		case ClosingState:
			fprintf(stderr, "Received READ event on closing socket\n");
			break;
	}
}

void Socket::notifyReadyWrite() {
	switch(getState()) {
		case UnconnectedState:
			fprintf(stderr, "Received WRITE event on closed socket\n");
			break;

		case HostLookupState:
			fprintf(stderr, "Received WRITE event on host lookup socket\n");
			break;

		case ConnectingState:
			setState(ConnectedState);
			break;

		case ConnectedState:
			break;

		case ClosingState:
			fprintf(stderr, "Received WRITE event on closing socket\n");
			break;
	}

}

void Socket::notifyReadyError(int errorValue) {
	std::unordered_map<void*, CallbackOnError>::const_iterator it, itEnd;
	for(it = _p->errorListeners.cbegin(), itEnd = _p->errorListeners.cend(); it != itEnd;) {
		void* instance = it->first;
		const CallbackOnError& callback = it->second;

		if(callback != nullptr) {
			(*callback)(instance, this, errorValue);
			++it;
		} else {
			it = _p->errorListeners.erase(it);
		}
	}
	abort();
}

int64_t Socket::getFd() {
	return _p->sock;
}

void Socket::setPoll(SocketPoll* socketPoll) {
	Socket::socketPoll = socketPoll;
}
