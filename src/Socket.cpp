#include "Socket.h"
#include "uv.h"
#include <stdio.h>
#include <string.h>
#include "CircularBuffer.h"
#include <unordered_map>
#include "IDelegate.h"

#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif

struct WriteRequest {
	uv_write_t writeReq;
	uv_buf_t buffer;
};

struct SocketInternal {
	IDelegate<Socket*> dataListeners;
	IDelegate<Socket*> incomingConnectionListeners;
	IDelegate<Socket*, Socket::State, Socket::State> eventListeners;
	IDelegate<Socket*, int> errorListeners;
	uv_tcp_t socket;
	uv_connect_t connectRequest;
	Socket::State currentState;
	CircularBuffer recvBuffer;
	bool sending;
};

Socket::Socket(uv_loop_t *uvLoop) : _p(new SocketInternal)
{
	this->uvLoop = uvLoop;
	_p->currentState = UnconnectedState;
	_p->connectRequest.data = this;
	_p->sending = false;
	_p->socket.data = this;
	lastError = 0;
	_p->recvBuffer.setBufferSize(16384);
}

Socket::~Socket() {
	if(getState() != UnconnectedState)
		abort();
	delete _p;
}

bool Socket::connect(const std::string & hostName, uint16_t port) {
	if(getState() != UnconnectedState)
		return false;

	this->host = hostName;
	this->port = port;
	setState(ConnectingState);

	uv_tcp_init(uvLoop, &_p->socket);

	struct sockaddr_in dest;

	uv_ip4_addr(hostName.c_str(), port, &dest);
	uv_tcp_connect(&_p->connectRequest, &_p->socket, (struct sockaddr*)&dest, &onConnected);

	return true;
}

bool Socket::listen(const std::string& interfaceIp, uint16_t port) {
	if(getState() != UnconnectedState)
		return false;

	setState(Binding);

	uv_tcp_init(uvLoop, &_p->socket);

	struct sockaddr_in bindAddr;

	uv_ip4_addr(interfaceIp.c_str(), port, &bindAddr);
	uv_tcp_bind(&_p->socket, (struct sockaddr*)&bindAddr);
	int r = uv_listen((uv_stream_t*)&_p->socket, 30, &onNewConnection);
	if(r < 0) {
		notifyReadyError(-r);
		return false;
	}

	setState(Listening);
	return true;
}

size_t Socket::read(void *buffer, size_t size) {
	return _p->recvBuffer.getData((char*)buffer, size);
}

size_t Socket::write(const void *buffer, size_t size) {
	WriteRequest* writeRequest = new WriteRequest;
	writeRequest->buffer.len = size;
	writeRequest->buffer.base = new char[writeRequest->buffer.len];
	writeRequest->writeReq.data = this;
	memcpy(writeRequest->buffer.base, buffer, size);
	uv_write(&writeRequest->writeReq, (uv_stream_t*)&_p->socket, &writeRequest->buffer, 1, &onWriteCompleted);

	return size;
}

bool Socket::accept(Socket* socket) {
	uv_tcp_t *client = &socket->_p->socket;
	uv_tcp_init(uvLoop, client);
	if (uv_accept((uv_stream_t*)&_p->socket, (uv_stream_t*) client) == 0) {
		struct sockaddr_in peerInfo;
		int sockAddrLen = sizeof(sockaddr_in);
		uv_tcp_getpeername(client, (struct sockaddr*)&peerInfo, &sockAddrLen);

		char ipBuffer[INET_ADDRSTRLEN];
		uv_ip4_name(&peerInfo, ipBuffer, INET_ADDRSTRLEN);
		socket->host = std::string(ipBuffer);
		socket->port = ntohs(peerInfo.sin_port);
		socket->_p->currentState = ConnectedState;
		uv_read_start((uv_stream_t*) client, &onAllocReceiveBuffer, &onReadCompleted);
	} else {
		return false;
	}

	return true;
}

void Socket::close() {
	if(getState() == ClosingState || getState() == UnconnectedState)
		return;

	setState(ClosingState);
	uv_shutdown_t shutdownReq;
	shutdownReq.data = this;
	uv_shutdown(&shutdownReq, (uv_stream_t*)&_p->socket, &onShutdownDone);
	while(getState() != UnconnectedState)
		uv_run(uvLoop, UV_RUN_ONCE);
}

void Socket::abort() {
	if(getState() == ClosingState || getState() == UnconnectedState)
		return;

	setState(ClosingState);
	uv_close((uv_handle_t*)&_p->socket, &onConnectionClosed);
	while(getState() != UnconnectedState)
		uv_run(uvLoop, UV_RUN_ONCE);
}

size_t Socket::getAvailableBytes() {
	return _p->recvBuffer.getAvailableBytes();
}

Socket::State Socket::getState() {
	return _p->currentState;
}

void Socket::setState(State state) {
	if(state == _p->currentState)
		return;


	State oldState = _p->currentState;

	_p->currentState = state;

	log("Socket state change from %d to %d\n", oldState, state);
	_p->eventListeners.dispatch(this, oldState, state);

	if(state == UnconnectedState) {
		this->host.clear();
		this->port = 0;
	}
}

DelegateRef Socket::addDataListener(void* instance, CallbackOnDataReady listener) {
	return _p->dataListeners.add(instance, listener);
}

DelegateRef Socket::addConnectionListener(void* instance, CallbackOnDataReady listener) {
	return _p->incomingConnectionListeners.add(instance, listener);
}

DelegateRef Socket::addEventListener(void* instance, CallbackOnStateChanged listener) {
	return _p->eventListeners.add(instance, listener);
}

DelegateRef Socket::addErrorListener(void* instance, CallbackOnError listener) {
	return _p->errorListeners.add(instance, listener);
}

void Socket::removeListener(void* instance) {
	_p->dataListeners.del(instance);
	_p->eventListeners.del(instance);
	_p->errorListeners.del(instance);
	_p->incomingConnectionListeners.del(instance);
}

void Socket::notifyReadyError(int errorValue) {
	_p->errorListeners.dispatch(this, errorValue);
	if(getState() != Listening)
		abort();
}

void Socket::onConnected(uv_connect_t* req, int status) {
	Socket* thisInstance = (Socket*)req->data;

	if(status < 0) {
		const char* errorString = uv_strerror(-status);
		thisInstance->error("Socket: %s\n", errorString);
		thisInstance->notifyReadyError(-status);
		return;
	}

	thisInstance->setState(ConnectedState);
	uv_read_start((uv_stream_t*)&thisInstance->_p->socket, &onAllocReceiveBuffer, &onReadCompleted);
}

void Socket::onNewConnection(uv_stream_t* req, int status) {
	Socket* thisInstance = (Socket*)req->data;

	if(status < 0) {
		const char* errorString = uv_strerror(-status);
		thisInstance->error("Socket: %s\n", errorString);
		thisInstance->notifyReadyError(-status);
		return;
	}

	thisInstance->_p->incomingConnectionListeners.dispatch(thisInstance);
}

void Socket::onAllocReceiveBuffer(uv_handle_t*, size_t, uv_buf_t* buf) {
	buf->base = new char[1024];
	buf->len = 1024;
}

void Socket::onReadCompleted(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
	Socket* thisInstance = (Socket*)stream->data;

	if(nread < 0) {
		if(buf->base)
			delete[] buf->base;

		const char* errorString = uv_strerror(-nread);
		thisInstance->error("Socket: %s\n", errorString);
		thisInstance->notifyReadyError(-nread);
	} else {
		thisInstance->log("Read %zd bytes\n", nread);
		thisInstance->_p->recvBuffer.insertData(buf->base, nread);
		delete[] buf->base;
		thisInstance->_p->dataListeners.dispatch(thisInstance);
	}
}

void Socket::onWriteCompleted(uv_write_t* req, int status) {
	WriteRequest* writeRequest = (WriteRequest*)req;
	Socket* thisInstance = (Socket*)req->data;


	if(status < 0) {
		const char* errorString = uv_strerror(-status);
		thisInstance->error("Socket: %s\n", errorString);
		thisInstance->notifyReadyError(-status);
	} else {
		thisInstance->log("Written %zd bytes\n", writeRequest->buffer.len);
	}

	delete[] writeRequest->buffer.base;
	delete writeRequest;
}

void Socket::onShutdownDone(uv_shutdown_t* req, int status) {
	Socket* thisInstance = (Socket*)req->data;

	if(status < 0) {
		const char* errorString = uv_strerror(-status);
		thisInstance->error("Socket: %s\n", errorString);
		return;
	}

	uv_close((uv_handle_t*)req->handle, &onConnectionClosed);
}

void Socket::onConnectionClosed(uv_handle_t* handle) {
	Socket* thisInstance = (Socket*)handle->data;

	thisInstance->setState(UnconnectedState);
}
