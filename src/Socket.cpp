#include "Socket.h"
#include "Log.h"
#include <sstream>
#include <iomanip>
#include <stdarg.h>
#include <string.h>

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

Socket::Socket(uv_loop_t *uvLoop, bool logPackets)
	: uvLoop(uvLoop),
	  remoteHost(0),
	  remotePort(0),
	  localHost(0),
	  localPort(0),
	  currentState(UnconnectedState),
	  socketInitialized(false),
	  packetLogger(nullptr),
	  logPackets(logPackets),
	  packetTransferedSinceLastCheck(true)
{
	remoteHostName[0] = localHostName[0] = 0;
	connectRequest.data = this;
	socket.data = this;
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

	strncpy(remoteHostName, hostName.c_str(), sizeof(remoteHostName));
	remotePort = port;
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


	strncpy(localHostName, interfaceIp.c_str(), sizeof(localHostName));
	localPort = port;
	setState(BindingState);

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

size_t Socket::discard(size_t size) {
	size_t sizeAvailable = recvBuffer.size();
	size_t effectiveSize = sizeAvailable > size ? size : sizeAvailable;

	recvBuffer.erase(recvBuffer.begin(), recvBuffer.begin() + effectiveSize);

	return effectiveSize;
}

size_t Socket::write(const void *buffer, size_t size) {
	if(getState() == ConnectedState) {
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
			return 0;
		}

		if(logPackets)
			packetLog(Log::LL_Debug, reinterpret_cast<const unsigned char*>(buffer), (int)size,
									"Written %d bytes\n",
									(int)size);

		return size;
	} else {
		error("Attempt to send data but socket not connected, current state is: %s(%d)\n", (getState() < (sizeof(STATES)/sizeof(const char*))) ? STATES[getState()] : "Unknown", getState());
		return 0;
	}

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

void Socket::retrieveSocketBoundsInfo() {
	// Retrieve remote bound infos
	struct sockaddr_in sockInfo;
	int sockAddrLen = sizeof(sockaddr_in);
	if(uv_tcp_getpeername(&socket, (struct sockaddr*)&sockInfo, &sockAddrLen) >= 0) {
		remoteHost = sockInfo.sin_addr.s_addr;
		uv_inet_ntop(AF_INET, &sockInfo.sin_addr.s_addr, remoteHostName, sizeof(remoteHostName));
		remotePort = ntohs(sockInfo.sin_port);
	} else {
		remoteHostName[0] = 0;
		remoteHost = 0;
		remotePort = 0;
	}

	// Retrieve local bound infos
	sockAddrLen = sizeof(sockaddr_in);
	if(uv_tcp_getsockname(&socket, (struct sockaddr*)&sockInfo, &sockAddrLen) >= 0) {
		localHost = sockInfo.sin_addr.s_addr;
		uv_inet_ntop(AF_INET, &sockInfo.sin_addr.s_addr, localHostName, sizeof(localHostName));
		localPort = ntohs(sockInfo.sin_port);
	} else {
		localHostName[0] = 0;
		localHost = 0;
		localPort = 0;
	}

	setDirtyObjectName();
}

void Socket::setState(State state) {
	if(state == currentState)
		return;

	if(state == ConnectedState || state == ListeningState) {
		retrieveSocketBoundsInfo();
	}

	State oldState = currentState;
	currentState = state;

	const char* oldStateStr = (oldState < (sizeof(STATES)/sizeof(const char*))) ? STATES[oldState] : "Unknown";
	const char* newStateStr = (state < (sizeof(STATES)/sizeof(const char*))) ? STATES[state] : "Unknown";
	trace("Socket state changed from %s to %s\n", oldStateStr, newStateStr);
	packetLog(Log::LL_Info, nullptr, 0, "Socket state changed from %s to %s\n", oldStateStr, newStateStr);

	DELEGATE_CALL(eventListeners, this, oldState, state);

	if(state == UnconnectedState) {
		remoteHostName[0] = 0;
		remoteHost = 0;
		remotePort = 0;
		localHostName[0] = 0;
		localHost = 0;
		localPort = 0;

		setDirtyObjectName();
	} else if(state == ConnectedState) {
		uv_read_start((uv_stream_t*) &socket, &onAllocReceiveBuffer, &onReadCompleted);
		packetTransferedSinceLastCheck = true;
	}

}

void Socket::updateObjectName() {
//	int nameSize = 0;
//	nameSize += getClassNameSize(); //name prefix is class name
//	nameSize += 1; // "["
//	nameSize += strlen(localHostName);
//	nameSize += 1; // ":"
//	nameSize += 5; // localPort
//	nameSize += 4; // " <> "
//	nameSize += strlen(remoteHostName);
//	nameSize += 1; // ":"
//	nameSize += 5; // remotePort
//	nameSize += 2; // "]\0"
	int nameSize = getClassNameSize() + strlen(localHostName) + strlen(remoteHostName) + 19;
	setObjectName(nameSize, "%s[%s:%u <> %s:%u]", getClassName(), localHostName, localPort, remoteHostName, remotePort);
}


void Socket::packetLog(Log::Level level, const unsigned char* rawData, int size, const char* format, ...) {
	static const char* char2hex[] = {
		"00","01","02","03","04","05","06","07","08","09","0A","0B","0C","0D","0E","0F",
		"10","11","12","13","14","15","16","17","18","19","1A","1B","1C","1D","1E","1F",
		"20","21","22","23","24","25","26","27","28","29","2A","2B","2C","2D","2E","2F",
		"30","31","32","33","34","35","36","37","38","39","3A","3B","3C","3D","3E","3F",
		"40","41","42","43","44","45","46","47","48","49","4A","4B","4C","4D","4E","4F",
		"50","51","52","53","54","55","56","57","58","59","5A","5B","5C","5D","5E","5F",
		"60","61","62","63","64","65","66","67","68","69","6A","6B","6C","6D","6E","6F",
		"70","71","72","73","74","75","76","77","78","79","7A","7B","7C","7D","7E","7F",
		"80","81","82","83","84","85","86","87","88","89","8A","8B","8C","8D","8E","8F",
		"90","91","92","93","94","95","96","97","98","99","9A","9B","9C","9D","9E","9F",
		"A0","A1","A2","A3","A4","A5","A6","A7","A8","A9","AA","AB","AC","AD","AE","AF",
		"B0","B1","B2","B3","B4","B5","B6","B7","B8","B9","BA","BB","BC","BD","BE","BF",
		"C0","C1","C2","C3","C4","C5","C6","C7","C8","C9","CA","CB","CC","CD","CE","CF",
		"D0","D1","D2","D3","D4","D5","D6","D7","D8","D9","DA","DB","DC","DD","DE","DF",
		"E0","E1","E2","E3","E4","E5","E6","E7","E8","E9","EA","EB","EC","ED","EE","EF",
		"F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","FA","FB","FC","FD","FE","FF"
	};

	if(packetLogger == nullptr || !packetLogger->wouldLog(level))
		return;

	char messageBuffer[4096];

	va_list args;

	va_start(args, format);
	vsnprintf(messageBuffer, 4096, format, args);
	messageBuffer[4095] = 0;
	va_end(args);

	if(rawData && size > 0) {
		std::string buffer;
		buffer.reserve((size+15)/16*74 + 1);

		//Log full packet data
		const int lineNum = (size+15)/16;

		for(int line = 0; line < lineNum; line++) {
			int maxCharNum = size - (line*16);
			if(maxCharNum > 16)
				maxCharNum = 16;

			buffer += char2hex[line*16 >> 8];
			buffer += char2hex[line*16 & 0xFF];
			buffer += ": ";

			for(int row = 0; row < 16; row++) {
				if(row < maxCharNum) {
					buffer += char2hex[rawData[line*16+row]];
					buffer += " ";
				} else
					buffer += "   ";
				if(row == 7)
					buffer += ' ';
			}

			buffer += ' ';

			for(int row = 0; row < maxCharNum; row++) {
				const unsigned char c = rawData[line*16+row];

				if(c >= 32 && c < 127)
					buffer += c;
				else
					buffer += '.';
				if(row == 7)
					buffer += ' ';
			}
			buffer += '\n';
		}

		packetLogger->log(level, this,
						  "%s%s\n",
						  messageBuffer,
						  buffer.c_str());
	} else {
		packetLogger->log(level, this,
						  "%s",
						  messageBuffer);
	}
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
		thisInstance->packetTransferedSinceLastCheck = true;
		thisInstance->trace("Read %ld bytes\n", (long)nread);

		if(thisInstance->logPackets)
			thisInstance->packetLog(Log::LL_Debug, reinterpret_cast<const unsigned char*>(buf->base), (int)nread,
									"Read    %ld bytes\n",  //large space to align with writes
									(long)nread);

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
		thisInstance->packetTransferedSinceLastCheck = true;
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
