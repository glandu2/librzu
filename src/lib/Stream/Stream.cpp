#include "Stream.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "Core/EventLoop.h"
#include "Core/Log.h"
#include "Pipe.h"
#include "Socket.h"

struct ReadBuffer {
	char data[1024];
	bool isUsed;
	bool mustBeDeleted;
};

const char* Stream::STATES[] = {"Unconnected", "Connecting", "Binding", "Listening", "Connected", "Closing"};

Stream::WriteRequest* Stream::WriteRequest::create(size_t dataSize) {
	WriteRequest* writeRequest = reinterpret_cast<WriteRequest*>(malloc(sizeof(WriteRequest) + dataSize));

	writeRequest->buffer.len = (int) dataSize;
	writeRequest->buffer.base = writeRequest->data;

	return writeRequest;
}

Stream::WriteRequest* Stream::WriteRequest::createFromExisting(char* buffer, size_t dataSize) {
	WriteRequest* writeRequest = reinterpret_cast<WriteRequest*>(malloc(sizeof(WriteRequest)));

	writeRequest->buffer.len = (int) dataSize;
	writeRequest->buffer.base = buffer;

	return writeRequest;
}

void Stream::WriteRequest::destroy(WriteRequest* req) {
	free(req);
}

Stream::Stream(uv_loop_t* uvLoop, uv_stream_t* handle, bool logPackets)
    : logPackets(logPackets),
      loop(uvLoop),
      handle(handle),
      currentState(UnconnectedState),
      packetLogger(Log::getDefaultPacketLogger()),
      packetTransferedSinceLastCheck(true),
      closeCausedByRemote(false) {
	connectRequest.data = this;
	handle->data = this;
}

Stream::~Stream() {
	if(getState() != UnconnectedState)
		abort();
	while(getState() != UnconnectedState)
		uv_run(getLoop(), UV_RUN_ONCE);
}

Stream::StreamType Stream::parseConnectionUrl(const char* url, std::string* target) {
	StreamType type = ST_Socket;
	const char* startOfTarget = url;

	if(strncmp(url, "pipe:", 5) == 0) {
		type = ST_Pipe;
		startOfTarget = url + 5;
	}

	if(target)
		target->assign(startOfTarget);

	return type;
}

Stream* Stream::getStream(StreamType type, Stream* existingStream, bool* changed, bool enablePacketLogger) {
	Stream* newStream = existingStream;

	switch(type) {
		case ST_Socket:
			if(!existingStream || existingStream->getTrueClassHash() != Socket::getClassHash()) {
				if(existingStream)
					existingStream->deleteLater();
				newStream = new Socket(EventLoop::getLoop(), enablePacketLogger);
			}
			break;

		case ST_Pipe:
			if(!existingStream || existingStream->getTrueClassHash() != Pipe::getClassHash()) {
				if(existingStream)
					existingStream->deleteLater();
				newStream = new Pipe(EventLoop::getLoop(), enablePacketLogger);
			}
			break;
	}

	if(changed)
		*changed = newStream != existingStream;

	return newStream;
}

bool Stream::connect(const std::string& hostName, uint16_t port) {
	if(getState() != UnconnectedState) {
		log(LL_Warning, "Attempt to connect a not unconnected stream to %s\n", hostName.c_str());
		return false;
	}

	setState(ConnectingState);

	int result = connect_impl(&connectRequest, hostName, port);
	if(result < 0) {
		log(LL_Warning, "Cant connect to host: %s\n", uv_strerror(result));
		onStreamError(result);
		return false;
	}

	return true;
}

bool Stream::listen(const std::string& interfaceIp, uint16_t port) {
	if(getState() != UnconnectedState) {
		log(LL_Warning, "Attempt to bind a not unconnected stream to %s\n", interfaceIp.c_str());
		return false;
	}

	setState(BindingState);

	int result = bind_impl(interfaceIp, port);
	if(result < 0) {
		log(LL_Warning, "Cant bind: %s\n", uv_strerror(result));
		onStreamError(result);
		return false;
	}

	setDirtyObjectName();

	result = uv_listen(handle, 20000, &onNewConnection);
	if(result < 0) {
		log(LL_Warning, "Cant listen: %s\n", uv_strerror(result));
		onStreamError(result);
		return false;
	}

	setState(ListeningState);
	return true;
}

size_t Stream::read(void* buffer, size_t size) {
	size_t sizeAvailable = recvBuffer.size();
	size_t effectiveSize = sizeAvailable > size ? size : sizeAvailable;

	memcpy(buffer, &recvBuffer[0], effectiveSize);
	recvBuffer.erase(recvBuffer.begin(), recvBuffer.begin() + effectiveSize);

	return effectiveSize;
}

size_t Stream::readAll(std::vector<char>* buffer) {
	buffer->swap(recvBuffer);
	recvBuffer.clear();
	// recvBuffer.reserve(buffer->capacity());

	return buffer->size();
}

size_t Stream::discard(size_t size) {
	size_t sizeAvailable = recvBuffer.size();
	size_t effectiveSize = sizeAvailable > size ? size : sizeAvailable;

	recvBuffer.erase(recvBuffer.begin(), recvBuffer.begin() + effectiveSize);

	return effectiveSize;
}

size_t Stream::discardAll() {
	size_t sizeAvailable = recvBuffer.size();

	recvBuffer.clear();

	return sizeAvailable;
}

size_t Stream::write(WriteRequest* writeRequest) {
	if(getState() == ConnectedState) {
		writeRequest->writeReq.data = this;
		int result = uv_write(&writeRequest->writeReq, handle, &writeRequest->buffer, 1, &onWriteCompleted);
		if(result < 0) {
			WriteRequest::destroy(writeRequest);
			log(LL_Debug, "Cant write: %s\n", uv_strerror(result));
			onStreamError(result);
			return 0;
		}

		if(logPackets)
			packetLog(LL_Debug,
			          reinterpret_cast<const unsigned char*>(writeRequest->buffer.base),
			          (int) writeRequest->buffer.len,
			          "Written %d bytes\n",
			          (int) writeRequest->buffer.len);

		return writeRequest->buffer.len;
	} else {
		log(LL_Error,
		    "Attempt to send data but stream not connected, current state is: %s(%d)\n",
		    (getState() < (sizeof(STATES) / sizeof(const char*))) ? STATES[getState()] : "Unknown",
		    getState());
		WriteRequest::destroy(writeRequest);
		return 0;
	}
}

size_t Stream::write(const void* buffer, size_t size) {
	if(getState() == ConnectedState) {
		WriteRequest* writeRequest = WriteRequest::create(size);
		memcpy(writeRequest->buffer.base, buffer, size);
		return write(writeRequest);
	} else {
		log(LL_Error,
		    "Attempt to send data but stream not connected, current state is: %s(%d)\n",
		    (getState() < (sizeof(STATES) / sizeof(const char*))) ? STATES[getState()] : "Unknown",
		    getState());
		return 0;
	}
}

bool Stream::accept(Stream** clientSocketPtr) {
	if(*clientSocketPtr == nullptr) {
		*clientSocketPtr = createStream_impl();
		(*clientSocketPtr)->setPacketLogger(packetLogger);
	}
	Stream* clientSocket = *clientSocketPtr;
	uv_stream_t* client = clientSocket->handle;

	int result = uv_accept(handle, client);
	if(result == UV_EAGAIN)
		return false;
	else if(result < 0) {
		log(LL_Debug, "Cant accept: %s\n", uv_strerror(result));
		onStreamError(result);
		return false;
	}

	clientSocket->currentState = UnconnectedState;
	clientSocket->setState(ConnectedState);

	return true;
}

void Stream::close(bool causedByRemote) {
	State state = getState();

	if(state == ClosingState || state == UnconnectedState)
		return;

	setState(ClosingState, causedByRemote);
	int result = UV_ENOTCONN;

	if(state == ConnectedState) {
		shutdownReq.data = this;
		result = uv_shutdown(&shutdownReq, handle, &onShutdownDone);
	}
	if(result < 0) {
		uv_close((uv_handle_t*) handle, &onConnectionClosed);
	}
}

void Stream::abort(bool causedByRemote) {
	if(getState() == ClosingState || getState() == UnconnectedState)
		return;

	setState(ClosingState, causedByRemote);
	uv_close((uv_handle_t*) handle, &onConnectionClosed);
}

void Stream::setState(State state, bool causedByRemote) {
	if(state == currentState)
		return;

	State oldState = currentState;
	currentState = state;

	DELEGATE_CALL(eventListeners, this, oldState, state, causedByRemote);

	if(state == ClosingState) {
		closeCausedByRemote = causedByRemote;
	} else if(state == ConnectedState) {
		uv_read_start(handle, &onAllocReceiveBuffer, &onReadCompleted);
		packetTransferedSinceLastCheck = true;
	}

	onStateChanged(oldState, state);

	const char* oldStateStr = (oldState < (sizeof(STATES) / sizeof(const char*))) ? STATES[oldState] : "Unknown";
	const char* newStateStr = (state < (sizeof(STATES) / sizeof(const char*))) ? STATES[state] : "Unknown";
	log(LL_Trace, "Stream state changed from %s to %s\n", oldStateStr, newStateStr);
	packetLog(LL_Info, nullptr, 0, "Stream state changed from %s to %s\n", oldStateStr, newStateStr);
}

void Stream::packetLog(Object::Level level, const unsigned char* rawData, int size, const char* format, ...) {
	static const char* char2hex[] = {
	    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F", "10", "11",
	    "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23",
	    "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31", "32", "33", "34", "35",
	    "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", "40", "41", "42", "43", "44", "45", "46", "47",
	    "48", "49", "4A", "4B", "4C", "4D", "4E", "4F", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
	    "5A", "5B", "5C", "5D", "5E", "5F", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B",
	    "6C", "6D", "6E", "6F", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D",
	    "7E", "7F", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
	    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F", "A0", "A1",
	    "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF", "B0", "B1", "B2", "B3",
	    "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF", "C0", "C1", "C2", "C3", "C4", "C5",
	    "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF", "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7",
	    "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF", "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9",
	    "EA", "EB", "EC", "ED", "EE", "EF", "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB",
	    "FC", "FD", "FE", "FF"};

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
		buffer.reserve((size + 15) / 16 * 74 + 1);

		// Log full packet data
		const int lineNum = (size + 15) / 16;

		for(int line = 0; line < lineNum; line++) {
			int maxCharNum = size - (line * 16);
			if(maxCharNum > 16)
				maxCharNum = 16;

			buffer += char2hex[(line * 16 >> 8) & 0xFF];
			buffer += char2hex[line * 16 & 0xFF];
			buffer += ": ";

			for(int row = 0; row < 16; row++) {
				if(row < maxCharNum) {
					buffer += char2hex[rawData[line * 16 + row]];
					buffer += " ";
				} else
					buffer += "   ";
				if(row == 7)
					buffer += ' ';
			}

			buffer += ' ';

			for(int row = 0; row < maxCharNum; row++) {
				const unsigned char c = rawData[line * 16 + row];

				if(c >= 32 && c < 127)
					buffer += c;
				else
					buffer += '.';
				if(row == 7)
					buffer += ' ';
			}
			buffer += '\n';
		}

		packetLogger->log(level, this, "%s%s\n", messageBuffer, buffer.c_str());
	} else {
		packetLogger->log(level, this, "%s", messageBuffer);
	}
}

void Stream::addDataListener(IListener* instance, CallbackOnDataReady listener) {
	return dataListeners.add(instance, listener);
}

void Stream::addConnectionListener(IListener* instance, CallbackOnDataReady listener) {
	return incomingConnectionListeners.add(instance, listener);
}

void Stream::addEventListener(IListener* instance, CallbackOnStateChanged listener) {
	return eventListeners.add(instance, listener);
}

void Stream::addErrorListener(IListener* instance, CallbackOnError listener) {
	return errorListeners.add(instance, listener);
}

void Stream::removeListener(IListener* instance) {
	dataListeners.del(instance);
	eventListeners.del(instance);
	errorListeners.del(instance);
	incomingConnectionListeners.del(instance);
}

void Stream::onStreamError(int errorValue) {
	DELEGATE_CALL(errorListeners, this, errorValue);
	if(getState() != ListeningState)
		abort(true);
}

void Stream::onConnected(uv_connect_t* req, int status) {
	Stream* thisInstance = (Stream*) req->data;

	if(status < 0) {
		const char* errorString = uv_strerror(status);
		thisInstance->log(LL_Error, "onConnected: %s\n", errorString);
		thisInstance->onStreamError(status);
		return;
	}
	thisInstance->setDirtyObjectName();

	thisInstance->setState(ConnectedState);
}

void Stream::onNewConnection(uv_stream_t* req, int status) {
	Stream* thisInstance = (Stream*) req->data;

	if(status < 0) {
		const char* errorString = uv_strerror(status);
		thisInstance->log(LL_Error, "onNewConnection: %s\n", errorString);
		thisInstance->onStreamError(status);
		return;
	}

	DELEGATE_CALL(thisInstance->incomingConnectionListeners, thisInstance);
}

void Stream::onAllocReceiveBuffer(uv_handle_t*, size_t, uv_buf_t* buf) {
	static ReadBuffer staticReadBuffer = {{0}, false, false};

#ifdef __GNUC__
	static_assert((void*) staticReadBuffer.data == (void*) &staticReadBuffer, "Expected ReadBuffer adresses wrong");
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

void Stream::onReadCompleted(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
	Stream* thisInstance = (Stream*) stream->data;

	if(nread == UV_EOF) {
		thisInstance->log(LL_Trace, "Connection closed by peer\n");
		thisInstance->close(true);
	} else if(nread == UV_ECONNRESET) {
		thisInstance->log(LL_Trace, "Connection reset by peer\n");
		thisInstance->abort(true);
	} else if(nread < 0) {
		const char* errorString = uv_strerror((int) nread);
		thisInstance->log(LL_Error, "onReadCompleted: %s\n", errorString);
		thisInstance->onStreamError((int) nread);
	} else if(nread > 0) {
		thisInstance->packetTransferedSinceLastCheck = true;
		thisInstance->log(LL_Trace, "Read %ld bytes\n", (long) nread);

		if(thisInstance->logPackets)
			thisInstance->packetLog(LL_Debug,
			                        reinterpret_cast<const unsigned char*>(buf->base),
			                        (int) nread,
			                        "Read    %ld bytes\n",  // large space to align with writes
			                        (long) nread);

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

void Stream::onWriteCompleted(uv_write_t* req, int status) {
	WriteRequest* writeRequest = (WriteRequest*) req;
	Stream* thisInstance = (Stream*) req->data;

	if(status < 0) {
		const char* errorString = uv_strerror(status);
		thisInstance->log(LL_Error, "onWriteCompleted: %s\n", errorString);
		thisInstance->onStreamError(status);
	} else {
		thisInstance->packetTransferedSinceLastCheck = true;
		thisInstance->log(LL_Trace, "Written %ld bytes\n", (long) writeRequest->buffer.len);
	}

	WriteRequest::destroy(writeRequest);
}

void Stream::onShutdownDone(uv_shutdown_t* req, int status) {
	Stream* thisInstance = (Stream*) req->data;

	if(status < 0) {
		const char* errorString = uv_strerror(status);
		thisInstance->log(LL_Error, "onShutdownDone: %s\n", errorString);
	}

	uv_close((uv_handle_t*) thisInstance->handle, &onConnectionClosed);
}

void Stream::onConnectionClosed(uv_handle_t* handle) {
	Stream* thisInstance = (Stream*) handle->data;

	thisInstance->setState(UnconnectedState, thisInstance->closeCausedByRemote);
	thisInstance->closeCausedByRemote = false;
}
