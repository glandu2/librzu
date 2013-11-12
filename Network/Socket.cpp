#include "Socket.h"
#include <sys/socket.h>

pthread_t Socket::pollthreadId = 0;
pthread_mutex_t Socket::mutex = PTHREAD_MUTEX_INITIALIZER;

Socket::Socket()
{
	pthread_mutex_lock(&mutex);
	if(pollthreadId == 0)
		pthread_create(&pollthreadId, NULL, &pollThread, NULL);
	pthread_mutex_unlock(&mutex);

	sock = 0;
	currentState = UnconnectedState;
}

Socket::~Socket() {

}

bool Socket::connect(const std::string & hostName, uint16_t port) {
	if(sock == 0 || currentState == UnconnectedState)
	sock = ::socket(AF_INET, SOCK_STREAM, 0);
}

size_t Socket::read(void *buffer, size_t size) {

}

size_t Socket::write(const void *buffer, size_t size) {

}

void Socket::close() {

}

void Socket::abort() {

}

Socket::State Socket::state() {

}


void Socket::addEventListener(ISocketListener *listener) {

}

void Socket::removeEventListener(ISocketListener *listener) {

}

