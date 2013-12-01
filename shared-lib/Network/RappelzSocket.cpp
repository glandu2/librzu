#include "RappelzSocket.h"
#include <openssl/evp.h>
#include <string.h>
#include "Packets/TS_SC_RESULT.h"
#include <iostream>

//#define NO_DEBUG

#define LOG_PREFIX "RappelzSocket: "

#ifdef NO_DEBUG
#define printf(...) (void)0
#endif

RappelzSocket::RappelzSocket() {

	inputBuffer.bufferSize = initialInputBufferSize;
	inputBuffer.buffer = new uint8_t[inputBuffer.bufferSize];
	inputBuffer.currentMessageSize = 0;

	addInstance(addDataListener(this, &dataReceived));
	addInstance(addErrorListener(this, &socketError));
	addInstance(addEventListener(this, &stateChanged));
}

RappelzSocket::~RappelzSocket() {
	abort();
	invalidateCallbacks();
	if(inputBuffer.buffer)
		delete inputBuffer.buffer;
}

bool RappelzSocket::connect(const std::string & hostName, uint16_t port) {
	this->host = hostName;
	this->port = port;
	return EncryptedSocket::connect(hostName, port);
}

void RappelzSocket::stateChanged(void* instance, ISocket*, ISocket::State oldState, ISocket::State newState) {
	RappelzSocket* thisInstance = static_cast<RappelzSocket*>(instance);

	if(newState == ISocket::ConnectedState) {
		printf(LOG_PREFIX"Socket %s:%d connected\n", thisInstance->host.c_str(), thisInstance->port);

		thisInstance->inputBuffer.currentMessageSize = 0;

		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		eventMsg.event = TS_CC_EVENT::CE_ServerConnected;
		thisInstance->dispatchPacket(&eventMsg);
	} else if(oldState == ISocket::ConnectedState) {
		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		eventMsg.event = TS_CC_EVENT::CE_ServerDisconnected;
		printf(LOG_PREFIX"Socket %s:%d disconnected\n", thisInstance->host.c_str(), thisInstance->port);

		thisInstance->dispatchPacket(&eventMsg);
	}
}

void RappelzSocket::socketError(void* instance, ISocket*, int errnoValue) {
	RappelzSocket* thisInstance = static_cast<RappelzSocket*>(instance);

	printf(LOG_PREFIX"Socket %s:%d socket error %s !\n", thisInstance->host.c_str(), thisInstance->port, strerror(errnoValue));

	if(thisInstance->getState() == ConnectingState) {
		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		eventMsg.event = TS_CC_EVENT::CE_ServerUnreachable;
		thisInstance->dispatchPacket(&eventMsg);
	}
}

void RappelzSocket::sendPacket(const TS_MESSAGE* data) {
	printf(LOG_PREFIX"Packet out id: %5d, size: %d\n", data->id, data->size);

	write(data, data->size);
}

void RappelzSocket::dispatchPacket(const TS_MESSAGE* packetData) {
	printf(LOG_PREFIX"Packet in id: %5d, size: %d\n", packetData->id, packetData->size);

	//packetListeners.dispatch(ALL_PACKETS, this, packetData);

	if(packetData->id != TS_SC_RESULT::packetID)
		packetListeners.dispatch(packetData->id, this, packetData);
	else
		packetListeners.dispatch(reinterpret_cast<const TS_SC_RESULT*>(packetData)->request_msg_id, this, packetData);
}

void RappelzSocket::dataReceived(void* instance, ISocket*) {
	RappelzSocket* thisInstance = static_cast<RappelzSocket*>(instance);
	InputBuffer* buffer = &thisInstance->inputBuffer;

	//std::cout << LOG_PREFIX << "Server: " << " Received data, size = " << thisInstance->getAvailableBytes() << "CurrentSize = " << buffer->currentMessageSize << std::endl;
	do {
		if(buffer->currentMessageSize == 0 && thisInstance->getAvailableBytes() < 4) {
			return;
		} else if(buffer->currentMessageSize == 0) {
			thisInstance->read(&buffer->currentMessageSize, 4);
			printf(LOG_PREFIX"New message received, size = %d, available: %lu\n", buffer->currentMessageSize, thisInstance->getAvailableBytes());
		}

		if(buffer->currentMessageSize != 0 && thisInstance->getAvailableBytes() >= (buffer->currentMessageSize - 4)) {
			if(buffer->currentMessageSize > buffer->bufferSize) {
				if(buffer->bufferSize)
					delete[] buffer->buffer;
				buffer->bufferSize = buffer->currentMessageSize;
				buffer->buffer = new uint8_t[buffer->currentMessageSize];
			}
			reinterpret_cast<TS_MESSAGE*>(buffer->buffer)->size = buffer->currentMessageSize;
			thisInstance->read(buffer->buffer + 4, buffer->currentMessageSize - 4);
			thisInstance->dispatchPacket(reinterpret_cast<TS_MESSAGE*>(buffer->buffer));

			buffer->currentMessageSize = 0;
		}
	} while((buffer->currentMessageSize == 0 && thisInstance->getAvailableBytes() >= 4) || (buffer->currentMessageSize != 0 && thisInstance->getAvailableBytes() >= (buffer->currentMessageSize - 4)));
}

DelegateRef RappelzSocket::addPacketListener(uint16_t packetId, void* instance, CallbackFunction onPacketReceivedCallback) {
	return packetListeners.add(packetId, instance, onPacketReceivedCallback);
}
