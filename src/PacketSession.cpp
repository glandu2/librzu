#include "PacketSession.h"
#include "EventLoop.h"
#include "RappelzServer.h"
#include "stdarg.h"

#include "Packets/PacketBaseMessage.h"
#include "Packets/TS_SC_RESULT.h"

PacketSession::PacketSession() {
	inputBuffer.bufferSize = INITIAL_INPUT_BUFFERSIZE;
	inputBuffer.buffer = new uint8_t[inputBuffer.bufferSize];
	inputBuffer.currentMessageSize = 0;
	inputBuffer.discardPacket = false;

}

PacketSession::~PacketSession() {
	delete[] inputBuffer.buffer;
}

void PacketSession::assignStream(Stream* stream) {
	SocketSession::assignStream(stream);
	stream->addErrorListener(this, &socketError);
}

void PacketSession::addPacketsToListen(int packetsIdNum, int firstPacketId, ...) {
	va_list l;
	va_start(l, firstPacketId);
	reserveCallbackCount(packetsIdNum);
	for(int i = 0; i < packetsIdNum; i++) {
		if(i != 0)
			addPacketListener(va_arg(l, int), this, &onPacketReceivedStatic);
		else
			addPacketListener(firstPacketId, this, &onPacketReceivedStatic);
	}
	va_end(l);
}

void PacketSession::onPacketReceivedStatic(IListener* instance, PacketSession*, const TS_MESSAGE* packet) {
	static_cast<PacketSession*>(instance)->onPacketReceived(packet);
}

void PacketSession::onStateChanged(Stream::State oldState, Stream::State newState) {
	if(newState == Stream::ConnectedState) {
		inputBuffer.currentMessageSize = 0;

		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		eventMsg.event = TS_CC_EVENT::CE_ServerConnected;
		dispatchPacket(&eventMsg);
	} else if(newState == Stream::UnconnectedState) {
		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		eventMsg.event = TS_CC_EVENT::CE_ServerDisconnected;

		dispatchPacket(&eventMsg);
	}
}

void PacketSession::socketError(IListener *instance, Stream*, int errnoValue) {
	PacketSession* thisInstance = static_cast<PacketSession*>(instance);

	if(thisInstance->getStream()->getState() == Stream::ConnectingState) {
		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		eventMsg.event = TS_CC_EVENT::CE_ServerUnreachable;
		thisInstance->dispatchPacket(&eventMsg);
	}
}

void PacketSession::sendPacket(const TS_MESSAGE* data) {
	write(data, data->size);

	//Log after for better latency
	logPacket(true, data);
}

void PacketSession::dispatchPacket(const TS_MESSAGE* packetData) {
	int callcount;

	//Log before to avoid having logging a packet send after having received this packet before logging this packet
	logPacket(false, packetData);

	//! onPacketReceived(packetData);

	if(packetData->id != TS_SC_RESULT::packetID)
		DELEGATE_HASH_CALL_GETNUM(callcount, packetListeners, packetData->id, this, packetData);
	else
		DELEGATE_HASH_CALL_GETNUM(callcount, packetListeners, reinterpret_cast<const TS_SC_RESULT*>(packetData)->request_msg_id, this, packetData);

//	if(callcount == 0)
//		CALLBACK_CALL(defaultPacketListener, this, packetData);
}

void PacketSession::logPacket(bool outgoing, const TS_MESSAGE* msg) {
	trace("Packet %s id: %5d, size: %d\n",
		  (outgoing)? "out" : " in",
		  msg->id,
		  int(msg->size - sizeof(TS_MESSAGE)));

	getStream()->packetLog(Log::LL_Debug, reinterpret_cast<const unsigned char*>(msg) + sizeof(TS_MESSAGE), (int)msg->size - sizeof(TS_MESSAGE),
			  "Packet %s id: %5d, size: %d\n",
			  (outgoing)? "out" : "in ",
			  msg->id,
			  int(msg->size - sizeof(TS_MESSAGE)));
}

void PacketSession::onDataReceived() {
	Stream* inputStream = getStream();
	InputBuffer* buffer = &inputBuffer;

	do {
		// if buffer->currentMessageSize == 0 => waiting for a new message
		if(buffer->currentMessageSize == 0 && inputStream->getAvailableBytes() < 4) {
			return;
		} else if(buffer->currentMessageSize == 0) {
			read(&buffer->currentMessageSize, 4);
			buffer->discardPacket = buffer->currentMessageSize > MAX_PACKET_SIZE;
		}

		if(buffer->currentMessageSize != 0 && buffer->discardPacket) {
			buffer->currentMessageSize -= (uint32_t) inputStream->discard(buffer->currentMessageSize);
		} else if(buffer->currentMessageSize != 0 && inputStream->getAvailableBytes() >= (buffer->currentMessageSize - 4)) {
			if(buffer->currentMessageSize > buffer->bufferSize) {
				if(buffer->bufferSize)
					delete[] buffer->buffer;
				buffer->bufferSize = buffer->currentMessageSize;
				buffer->buffer = new uint8_t[buffer->currentMessageSize];
			}
			reinterpret_cast<TS_MESSAGE*>(buffer->buffer)->size = buffer->currentMessageSize;
			read(buffer->buffer + 4, buffer->currentMessageSize - 4);
			dispatchPacket(reinterpret_cast<TS_MESSAGE*>(buffer->buffer));

			buffer->currentMessageSize = 0;
		}
	} while((buffer->currentMessageSize == 0 && inputStream->getAvailableBytes() >= 4) || (buffer->currentMessageSize != 0 && inputStream->getAvailableBytes() >= (buffer->currentMessageSize - 4)));
}

void PacketSession::addPacketListener(uint16_t packetId, IListener* instance, CallbackFunction onPacketReceivedCallback) {
	packetListeners.add(packetId, instance, onPacketReceivedCallback);
}

void PacketSession::removePacketListener(uint16_t packetId, IListener* instance) {
	packetListeners.del(packetId, instance);
}
