#include "Socket.h"

#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif

struct WriteRequest {
	uv_write_t writeReq;
	uv_buf_t buffer;
	char data[];
};

struct ReadBuffer {
	char data[1024];
	bool isUsed;
	bool mustBeDeleted;
};

const char* Socket::STATES[] = { "Unconnected", "Connecting", "Binding", "Listening", "Connected", "Closing" };

Socket::Socket(uv_loop_t *uvLoop)
{
	this->uvLoop = uvLoop;
	socketInitialized = false;
	currentState = UnconnectedState;
	connectRequest.data = this;
	socket.data = this;
	port = 0;
}

Socket::~Socket() {
	if(getState() != UnconnectedState)
		abort();
	while(getState() != UnconnectedState)
		uv_run(uvLoop, UV_RUN_ONCE);
}

bool Socket::connect(const std::string & hostName, uint16_t port) {
	if(getState() != UnconnectedState) {
		warn("Attempt to connect a not unconnected socket to %s:%d\n", hostName.c_str(), port);
		return false;
	}

	setPeerInfo(hostName, port);
	setState(ConnectingState);

	uv_tcp_init(uvLoop, &socket);

	struct sockaddr_in dest;

	uv_ip4_addr(hostName.c_str(), port, &dest);
	int result = uv_tcp_connect(&connectRequest, &socket, (struct sockaddr*)&dest, &onConnected);
	if(result < 0) {
		warn("Cant connect to host: %s\n", uv_strerror(result));
		notifyReadyError(result);
		return false;
	}

	return true;
}

bool Socket::listen(const std::string& interfaceIp, uint16_t port) {
	if(getState() != UnconnectedState) {
		warn("Attempt to bind a not unconnected socket to %s:%d\n", interfaceIp.c_str(), port);
		return false;
	}

	setState(BindingState);

	setPeerInfo(interfaceIp, port);
	uv_tcp_init(uvLoop, &socket);

	struct sockaddr_in bindAddr;

	uv_ip4_addr(interfaceIp.c_str(), port, &bindAddr);
	uv_tcp_bind(&socket, (struct sockaddr*)&bindAddr, 0);
	int result = uv_listen((uv_stream_t*)&socket, 20000, &onNewConnection);
	if(result < 0) {
		warn("Cant listen: %s\n", uv_strerror(result));
		notifyReadyError(result);
		return false;
	}

	setState(ListeningState);
	return true;
}

size_t Socket::read(void *buffer, size_t size) {
	size_t sizeAvailable = recvBuffer.size();
	size_t effectiveSize = sizeAvailable > size ? size : sizeAvailable;

	memcpy(buffer, &recvBuffer[0], effectiveSize);
	recvBuffer.erase(recvBuffer.begin(), recvBuffer.begin() + effectiveSize);

	return effectiveSize;
}

size_t Socket::readAll(std::vector<char> *buffer) {
	buffer->swap(recvBuffer);
	recvBuffer.clear();
	//recvBuffer.reserve(buffer->capacity());

	return buffer->size();
}

size_t Socket::write(const void *buffer, size_t size) {
	WriteRequest* writeRequest = (WriteRequest*)new char[sizeof(WriteRequest) + size];
	writeRequest->buffer.len = size;
	writeRequest->buffer.base = writeRequest->data;
	writeRequest->writeReq.data = this;
	memcpy(writeRequest->buffer.base, buffer, size);
	int result = uv_write(&writeRequest->writeReq, (uv_stream_t*)&socket, &writeRequest->buffer, 1, &onWriteCompleted);
	if(result < 0) {
		delete[] (char*)writeRequest;
		debug("Cant write: %s\n", uv_strerror(result));
		notifyReadyError(result);
		return false;
	}

	return size;
}

bool Socket::accept(Socket* clientSocket) {
	uv_tcp_t *client = &clientSocket->socket;

	if(clientSocket->socketInitialized == false) {
		clientSocket->socketInitialized = true;
		uv_tcp_init(uvLoop, client);
	}

	int result = uv_accept((uv_stream_t*)&socket, (uv_stream_t*) client);
	if(result == UV_EAGAIN)
		return false;
	else if(result < 0) {
		debug("Cant accept: %s\n", uv_strerror(result));
		notifyReadyError(result);
		return false;
	}

	clientSocket->currentState = UnconnectedState;

	struct sockaddr_in peerInfo = clientSocket->getPeerInfo();
	char buffer[INET_ADDRSTRLEN] = {0};
	uv_inet_ntop(AF_INET, &peerInfo.sin_addr, buffer, INET_ADDRSTRLEN);
	clientSocket->setPeerInfo(std::string(buffer), ntohs(peerInfo.sin_port));
	clientSocket->setState(ConnectedState);

	return true;
}

void Socket::close() {
	if(getState() == ClosingState || getState() == UnconnectedState)
		return;

	setState(ClosingState);
	int result = UV_ENOTCONN;

	if(getState() == ConnectedState) {
		shutdownReq.data = this;
		result = uv_shutdown(&shutdownReq, (uv_stream_t*)&socket, &onShutdownDone);
	}
	if(result < 0) {
		uv_close((uv_handle_t*)&socket, &onConnectionClosed);
	}
}

void Socket::abort() {
	if(getState() == ClosingState || getState() == UnconnectedState)
		return;

	setState(ClosingState);
	uv_close((uv_handle_t*)&socket, &onConnectionClosed);
}

struct sockaddr_in Socket::getPeerInfo() {
	struct sockaddr_in peerInfo;
	int sockAddrLen = sizeof(sockaddr_in);
	uv_tcp_getpeername(&socket, (struct sockaddr*)&peerInfo, &sockAddrLen);

	return peerInfo;
}

void Socket::setState(State state) {
	if(state == currentState)
		return;

	State oldState = currentState;
	currentState = state;

	const char* oldStateStr = (oldState < (sizeof(STATES)/sizeof(const char*))) ? STATES[oldState] : "Unknown";
	const char* newStateStr = (oldState < (sizeof(STATES)/sizeof(const char*))) ? STATES[state] : "Unknown";
	trace("Socket state change from %s to %s\n", oldStateStr, newStateStr);

	DELEGATE_CALL(eventListeners, this, oldState, state);

	if(state == UnconnectedState) {
		this->host.clear();
		this->port = 0;
		setObjectName(nullptr);
	} else if(state == ConnectedState) {
		uv_read_start((uv_stream_t*) &socket, &onAllocReceiveBuffer, &onReadCompleted);
	}
}

void Socket::setPeerInfo(const std::string& host, uint16_t port) {
	this->host = host;
	this->port = port;
	setObjectName(getObjectNameSize() + 3 + host.size() + 5, "%s[%s:%u]", getObjectName(), host.c_str(), port);
}

void Socket::addDataListener(IListener* instance, CallbackOnDataReady listener) {
	return dataListeners.add(instance, listener);
}

void Socket::addConnectionListener(IListener* instance, CallbackOnDataReady listener) {
	return incomingConnectionListeners.add(instance, listener);
}

void Socket::addEventListener(IListener* instance, CallbackOnStateChanged listener) {
	return eventListeners.add(instance, listener);
}

void Socket::addErrorListener(IListener* instance, CallbackOnError listener) {
	return errorListeners.add(instance, listener);
}

void Socket::removeListener(IListener* instance) {
	dataListeners.del(instance);
	eventListeners.del(instance);
	errorListeners.del(instance);
	incomingConnectionListeners.del(instance);
}

void Socket::notifyReadyError(int errorValue) {
	DELEGATE_CALL(errorListeners, this, errorValue);
	if(getState() != ListeningState)
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

	DELEGATE_CALL(thisInstance->incomingConnectionListeners, thisInstance);
}

void Socket::onAllocReceiveBuffer(uv_handle_t*, size_t, uv_buf_t* buf) {
	static ReadBuffer staticReadBuffer = {{0}, false, false};

#ifdef __GNUC__
	static_assert((void*)staticReadBuffer.data == (void*)&staticReadBuffer, "Expected ReadBuffer adresses wrong");
#endif

	if(staticReadBuffer.isUsed == false) {
		staticReadBuffer.isUsed = true;
		buf->base = staticReadBuffer.data;
		buf->len = sizeof(staticReadBuffer.data);
	} else {
		ReadBuffer* buffer = new ReadBuffer;
		buffer->isUsed = true;
		buffer->mustBeDeleted = true;
		buf->base = buffer->data;
		buf->len = sizeof(buffer->data);
	}
}

void Socket::onReadCompleted(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
	Socket* thisInstance = (Socket*)stream->data;

	if(nread == UV_EOF) {
		thisInstance->trace("Connection closed by peer\n");
		thisInstance->close();
	} else if(nread == UV_ECONNRESET) {
		thisInstance->trace("Connection reset by peer\n");
		thisInstance->abort();
	} else if(nread < 0) {
		const char* errorString = uv_strerror(nread);
		thisInstance->error("onReadCompleted: %s\n", errorString);
		thisInstance->notifyReadyError(nread);
	} else {
		thisInstance->trace("Read %ld bytes\n", (long)nread);
		size_t oldSize = thisInstance->recvBuffer.size();
		thisInstance->recvBuffer.resize(oldSize + nread);
		memcpy(&thisInstance->recvBuffer[oldSize], buf->base, nread);
		DELEGATE_CALL(thisInstance->dataListeners, thisInstance);
	}

	if(buf) {
		ReadBuffer* buffer = (ReadBuffer*) buf->base;
		if(buffer) {
			buffer->isUsed = false;
			if(buffer->mustBeDeleted)
				delete buffer;
		}
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
		thisInstance->trace("Written %ld bytes\n", (long)writeRequest->buffer.len);
	}

//	delete[] writeRequest->buffer.base;
//	delete writeRequest;
	delete[] (char*)writeRequest;
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
