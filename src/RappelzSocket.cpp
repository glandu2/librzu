#include "RappelzSocket.h"
#include <string.h>
#include "Packets/TS_SC_RESULT.h"
#include "Log.h"

RappelzSocket::RappelzSocket(uv_loop_t* uvLoop, Mode mode)
	: EncryptedSocket(uvLoop, mode)
{

	inputBuffer.bufferSize = INITIAL_INPUT_BUFFERSIZE;
	inputBuffer.buffer = new uint8_t[inputBuffer.bufferSize];
	inputBuffer.currentMessageSize = 0;
	inputBuffer.discardPacket = false;

	addDataListener(this, &dataReceived);
	addErrorListener(this, &socketError);
	addEventListener(this, &stateChanged);
}

RappelzSocket::~RappelzSocket() {
	abort();
	invalidateCallbacks();
	if(inputBuffer.buffer)
		delete[] inputBuffer.buffer;
}

void RappelzSocket::stateChanged(IListener* instance, Socket*, Socket::State oldState, Socket::State newState) {
	RappelzSocket* thisInstance = static_cast<RappelzSocket*>(instance);

	if(newState == Socket::ConnectedState) {
		thisInstance->inputBuffer.currentMessageSize = 0;

		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		eventMsg.event = TS_CC_EVENT::CE_ServerConnected;
		thisInstance->dispatchPacket(&eventMsg);
	} else if(newState == Socket::UnconnectedState) {
		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		eventMsg.event = TS_CC_EVENT::CE_ServerDisconnected;

		thisInstance->dispatchPacket(&eventMsg);
	}
}

void RappelzSocket::socketError(IListener *instance, Socket*, int errnoValue) {
	RappelzSocket* thisInstance = static_cast<RappelzSocket*>(instance);

	if(thisInstance->getState() == ConnectingState) {
		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		eventMsg.event = TS_CC_EVENT::CE_ServerUnreachable;
		thisInstance->dispatchPacket(&eventMsg);
	}
}

void RappelzSocket::sendPacket(const TS_MESSAGE* data) {
	write(data, data->size);

	//Log after for better latency
	logPacket(true, data);
}

void RappelzSocket::dispatchPacket(const TS_MESSAGE* packetData) {
	int callcount;

	//Log before to avoid having logging a packet send after having received this packet before logging this packet
	logPacket(false, packetData);

	if(packetData->id != TS_SC_RESULT::packetID)
		DELEGATE_HASH_CALL_GETNUM(callcount, packetListeners, packetData->id, this, packetData);
	else
		DELEGATE_HASH_CALL_GETNUM(callcount, packetListeners, reinterpret_cast<const TS_SC_RESULT*>(packetData)->request_msg_id, this, packetData);

	if(callcount == 0)
		CALLBACK_CALL(defaultPacketListener, this, packetData);
}

void RappelzSocket::logPacket(bool outgoing, const TS_MESSAGE* msg) {
	trace("Packet %s id: %5d, size: %d\n",
		  (outgoing)? "out" : " in",
		  msg->id,
		  msg->size);

	packetLog(Log::LL_Info, reinterpret_cast<const unsigned char*>(msg), (int)msg->size,
			  "Packet %s id: %5d, size: %d\n",
			  (outgoing)? "out" : "in ",
			  msg->id,
			  msg->size);
}

void RappelzSocket::dataReceived(IListener* instance, Socket*) {
	RappelzSocket* thisInstance = static_cast<RappelzSocket*>(instance);
	InputBuffer* buffer = &thisInstance->inputBuffer;

	//std::cout << LOG_PREFIX << "Server: " << " Received data, size = " << thisInstance->getAvailableBytes() << "CurrentSize = " << buffer->currentMessageSize << std::endl;
	do {
		// if buffer->currentMessageSize == 0 => waiting for a new message
		if(buffer->currentMessageSize == 0 && thisInstance->getAvailableBytes() < 4) {
			return;
		} else if(buffer->currentMessageSize == 0) {
			thisInstance->read(&buffer->currentMessageSize, 4);
			buffer->discardPacket = buffer->currentMessageSize > MAX_PACKET_SIZE;
			//trace("Receiving new msg, size = %d, available: %lu\n", buffer->currentMessageSize, thisInstance->getAvailableBytes());
		}

		if(buffer->currentMessageSize != 0 && buffer->discardPacket) {
			buffer->currentMessageSize -= (uint32_t) thisInstance->discard(buffer->currentMessageSize);
		} else if(buffer->currentMessageSize != 0 && thisInstance->getAvailableBytes() >= (buffer->currentMessageSize - 4)) {
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

void RappelzSocket::addPacketListener(uint16_t packetId, IListener* instance, CallbackFunction onPacketReceivedCallback) {
	packetListeners.add(packetId, instance, onPacketReceivedCallback);
}

void RappelzSocket::removePacketListener(uint16_t packetId, IListener* instance) {
	packetListeners.del(packetId, instance);
}

void RappelzSocket::setUnknownPacketListener(IListener* instance, CallbackFunction onPacketReceivedCallback) {
	defaultPacketListener = Callback<CallbackFunction>(instance, onPacketReceivedCallback);
}
