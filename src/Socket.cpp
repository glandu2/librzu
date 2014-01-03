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
	IDelegate<Socket::CallbackOnDataReady> dataListeners;
	IDelegate<Socket::CallbackOnDataReady> incomingConnectionListeners;
	IDelegate<Socket::CallbackOnStateChanged> eventListeners;
	IDelegate<Socket::CallbackOnError> errorListeners;
	uv_tcp_t socket;
	uv_connect_t connectRequest;
	uv_shutdown_t shutdownReq;
	Socket::State currentState;
	CircularBuffer recvBuffer;
	bool sending;
};

const char* Socket::STATES[] = { "UnconnectedState", "ConnectingState", "Binding", "Listening", "ConnectedState", "ClosingState" };

Socket::Socket(uv_loop_t *uvLoop) : _p(new SocketInternal)
{
	this->uvLoop = uvLoop;
	_p->currentState = UnconnectedState;
	_p->connectRequest.data = this;
	_p->sending = false;
	_p->socket.data = this;
	lastError = 0;
}

Socket::~Socket() {
	if(getState() != UnconnectedState)
		abort();
	while(getState() != UnconnectedState)
		uv_run(uvLoop, UV_RUN_ONCE);
	delete _p;
}

bool Socket::connect(const std::string & hostName, uint16_t port) {
	if(getState() != UnconnectedState)
		return false;

	setPeerInfo(hostName, port);
	setState(ConnectingState);

	uv_tcp_init(uvLoop, &_p->socket);

	struct sockaddr_in dest;

	uv_ip4_addr(hostName.c_str(), port, &dest);
	int result = uv_tcp_connect(&_p->connectRequest, &_p->socket, (struct sockaddr*)&dest, &onConnected);
	if(result < 0) {
		debug("Cant connect to host: %s\n", uv_strerror(result));
		notifyReadyError(result);
		return false;
	}

	return true;
}

bool Socket::listen(const std::string& interfaceIp, uint16_t port) {
	if(getState() != UnconnectedState)
		return false;

	setState(Binding);

	setPeerInfo(interfaceIp, port);
	uv_tcp_init(uvLoop, &_p->socket);

	struct sockaddr_in bindAddr;

	uv_ip4_addr(interfaceIp.c_str(), port, &bindAddr);
	uv_tcp_bind(&_p->socket, (struct sockaddr*)&bindAddr);
	int result = uv_listen((uv_stream_t*)&_p->socket, 30, &onNewConnection);
	if(result < 0) {
		debug("Cant listen: %s\n", uv_strerror(result));
		notifyReadyError(result);
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
	int result = uv_write(&writeRequest->writeReq, (uv_stream_t*)&_p->socket, &writeRequest->buffer, 1, &onWriteCompleted);
	if(result < 0) {
		delete[] writeRequest->buffer.base;
		delete writeRequest;
		debug("Cant write: %s\n", uv_strerror(result));
		notifyReadyError(result);
		return false;
	}

	return size;
}

bool Socket::accept(Socket* socket) {
	uv_tcp_t *client = &socket->_p->socket;
	uv_tcp_init(uvLoop, client);
	int result = uv_accept((uv_stream_t*)&_p->socket, (uv_stream_t*) client);
	if(result < 0) {
		debug("Cant accept: %s\n", uv_strerror(result));
		notifyReadyError(result);
		return false;
	}

	struct sockaddr_in peerInfo;
	int sockAddrLen = sizeof(sockaddr_in);
	uv_tcp_getpeername(client, (struct sockaddr*)&peerInfo, &sockAddrLen);

	char ipBuffer[INET_ADDRSTRLEN];
	uv_ip4_name(&peerInfo, ipBuffer, INET_ADDRSTRLEN);
	socket->setPeerInfo(std::string(ipBuffer), ntohs(peerInfo.sin_port));
	socket->_p->currentState = UnconnectedState;
	socket->setState(ConnectedState);

	return true;
}

void Socket::close() {
	if(getState() == ClosingState || getState() == UnconnectedState)
		return;

	setState(ClosingState);
	_p->shutdownReq.data = this;
	int result = uv_shutdown(&_p->shutdownReq, (uv_stream_t*)&_p->socket, &onShutdownDone);
	if(result < 0) {
		uv_close((uv_handle_t*)&_p->socket, &onConnectionClosed);
	}
}

void Socket::abort() {
	if(getState() == ClosingState || getState() == UnconnectedState)
		return;

	setState(ClosingState);
	uv_close((uv_handle_t*)&_p->socket, &onConnectionClosed);
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

	const char* oldStateStr = (oldState < (sizeof(STATES)/sizeof(const char*))) ? STATES[oldState] : "Unknown";
	const char* newStateStr = (oldState < (sizeof(STATES)/sizeof(const char*))) ? STATES[state] : "Unknown";
	debug("Socket state change from %s to %s\n", oldStateStr, newStateStr);

	DELEGATE_CALL(_p->eventListeners, this, oldState, state);

	if(state == UnconnectedState) {
		this->host.clear();
		this->port = 0;
		setObjectName(nullptr);
	} else if(state == ConnectedState) {
		_p->recvBuffer.setBufferSize(16384);
		uv_read_start((uv_stream_t*) &_p->socket, &onAllocReceiveBuffer, &onReadCompleted);
	}
}

void Socket::setPeerInfo(const std::string& host, uint16_t port) {
	this->host = host;
	this->port = port;
	setObjectName(getObjectNameSize() + 3 + host.size() + 5, "%s[%s:%u]", getObjectName(), host.c_str(), port);
}

void Socket::addDataListener(ICallbackGuard* instance, CallbackOnDataReady listener) {
	return _p->dataListeners.add(instance, listener);
}

void Socket::addConnectionListener(ICallbackGuard* instance, CallbackOnDataReady listener) {
	return _p->incomingConnectionListeners.add(instance, listener);
}

void Socket::addEventListener(ICallbackGuard* instance, CallbackOnStateChanged listener) {
	return _p->eventListeners.add(instance, listener);
}

void Socket::addErrorListener(ICallbackGuard* instance, CallbackOnError listener) {
	return _p->errorListeners.add(instance, listener);
}

void Socket::removeListener(ICallbackGuard* instance) {
	_p->dataListeners.del(instance);
	_p->eventListeners.del(instance);
	_p->errorListeners.del(instance);
	_p->incomingConnectionListeners.del(instance);
}

void Socket::notifyReadyError(int errorValue) {
	DELEGATE_CALL(_p->errorListeners, this, errorValue);
	if(getState() != Listening)
		abort();
}

void Socket::onConnected(uv_connect_t* req, int status) {
	Socket* thisInstance = (Socket*)req->data;

	if(status < 0) {
		const char* errorString = uv_strerror(status);
		thisInstance->error("onConnected: %s\n", errorString);
		thisInstance->notifyReadyError(status);
		return;
	}

	thisInstance->setState(ConnectedState);
}

void Socket::onNewConnection(uv_stream_t* req, int status) {
	Socket* thisInstance = (Socket*)req->data;

	if(status < 0) {
		const char* errorString = uv_strerror(status);
		thisInstance->error("onNewConnection: %s\n", errorString);
		thisInstance->notifyReadyError(status);
		return;
	}

	DELEGATE_CALL(thisInstance->_p->incomingConnectionListeners, thisInstance);
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

		const char* errorString = uv_strerror(nread);
		thisInstance->error("onReadCompleted: %s\n", errorString);
		thisInstance->notifyReadyError(nread);
	} else {
		thisInstance->debug("Read %ld bytes\n", (long)nread);
		thisInstance->_p->recvBuffer.insertData(buf->base, nread);
		delete[] buf->base;
		DELEGATE_CALL(thisInstance->_p->dataListeners, thisInstance);
	}
}

void Socket::onWriteCompleted(uv_write_t* req, int status) {
	WriteRequest* writeRequest = (WriteRequest*)req;
	Socket* thisInstance = (Socket*)req->data;


	if(status < 0) {
		const char* errorString = uv_strerror(status);
		thisInstance->error("onWriteCompleted: %s\n", errorString);
		thisInstance->notifyReadyError(status);
	} else {
		thisInstance->debug("Written %ld bytes\n", (long)writeRequest->buffer.len);
	}

	delete[] writeRequest->buffer.base;
	delete writeRequest;
}

void Socket::onShutdownDone(uv_shutdown_t* req, int status) {
	Socket* thisInstance = (Socket*)req->data;

	if(status < 0) {
		const char* errorString = uv_strerror(status);
		thisInstance->error("onShutdownDone: %s\n", errorString);
	}

	uv_close((uv_handle_t*)req->handle, &onConnectionClosed);
}

void Socket::onConnectionClosed(uv_handle_t* handle) {
	Socket* thisInstance = (Socket*)handle->data;

	thisInstance->setState(UnconnectedState);
}
