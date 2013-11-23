/*#include "Socket.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

pthread_t Socket::pollthreadId = 0;
pthread_mutex_t Socket::mutex = PTHREAD_MUTEX_INITIALIZER;

#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif

enum State {
	UnconnectedState,
	HostLookupState,
	ConnectingState,
	ConnectedState,
	ClosingState
};

Socket::Socket()
{
	pthread_mutex_lock(&mutex);
	if(pollthreadId == 0) {
		pthread_create(&pollthreadId, NULL, &pollThread, NULL);
	}
	pthread_mutex_unlock(&mutex);

	sock = 0;
	currentState = UnconnectedState;
}

Socket::~Socket() {
	if(currentState != UnconnectedState)
		abort();
}

bool Socket::connect(const std::string & hostName, uint16_t port) {
	if(currentState != UnconnectedState)
		return false;

	currentState = ConnectingState;

	if(sock != 0)
		abort();

	sock = ::socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in hostAddress;
	struct addrinfo hints, *servinfo, *p;
    int rv, lastError;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;

	if( (rv = getaddrinfo( hostname , NULL, &hints , &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		hostAddress = (struct sockaddr_in) *p->ai_addr;
		hostAddress.sin_port = htons(port);
		if( (lastError = ::connect(sock, &hostAddress, sizeof(hostAddress))) )
			break;
	}

	if(lastError) {
		close();
		return false;
	} else {
		currentState = ConnectedState;
		return true;
	}
}

int Socket::read(void *buffer, size_t size) {
	size_t totalByteRead;
	int byteRead;

	for(totalByteRead = 0; totalByteRead < size;) {
		byteRead = ::recv(sock, buffer + totalByteRead, size - totalByteRead, 0);
		if(byteRead < 0) {
			break;
		}
		totalByteRead += byteRead;
	}

	if(totalByteRead != size) {
		int socketError, errorSize = sizeof(int);
		::getsockopt(sock, SOL_SOCKET, SO_ERROR, &socketError, &errorSize);
		if(socketError) {
			fprintf(stderr, "Failed to recv from socket: %s\n", strerror(socketError));
			close();
			return socketError;
		}
	}

	return 0;
}

int Socket::write(const void *buffer, size_t size) {
	size_t totalByteWritten;
	int byteWritten;

	for(byteWritten = 0; byteWritten < size;) {
		byteWritten = ::send(sock, buffer + totalByteWritten, size - totalByteWritten, 0);
		if(byteWritten < 0) {
			break;
		}
		totalByteWritten += byteWritten;
	}

	if(totalByteWritten != size) {
		int socketError, errorSize = sizeof(int);
		::getsockopt(sock, SOL_SOCKET, SO_ERROR, &socketError, &errorSize);
		if(socketError) {
			fprintf(stderr, "Failed to send through socket: %s\n", strerror(socketError));
			close();
			return socketError;
		}
	}

	return 0;
}

void Socket::close() {
	currentState = ClosingState;
	::shutdown(sock, SHUT_RDWR);
	::close(sock);
	sock = 0;
	currentState = UnconnectedState;
}

void Socket::abort() {
	int lingerValue = 0;
	int lingerOptionSize = sizeof(int);
	::setsockopt(sock, SOL_SOCKET, SO_LINGER, &lingerValue, lingerOptionSize);
	close();
}

Socket::State Socket::state() {
	return currentState;
}


void Socket::addEventListener(ISocketListener *listener) {

}

void Socket::removeEventListener(ISocketListener *listener) {

}

*/
