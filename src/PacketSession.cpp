#include "PacketSession.h"

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

void PacketSession::sendPacket(const TS_MESSAGE* data) {
	write(data, data->size);

	//Log after for better latency
	logPacket(true, data);
}

void PacketSession::onStateChanged(Stream::State oldState, Stream::State newState, bool causedByRemote) {
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

void PacketSession::onError(int) {
	if(getStream()->getState() == Stream::ConnectingState) {
		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		eventMsg.event = TS_CC_EVENT::CE_ServerUnreachable;
		dispatchPacket(&eventMsg);
	}
}

void PacketSession::dispatchPacket(const TS_MESSAGE* packetData) {
	//Log before to avoid having logging a packet send after having received this packet before logging this packet
	logPacket(false, packetData);

	onPacketReceived(packetData);
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
			if(buffer->currentMessageSize <= 4)
				buffer->currentMessageSize = 0;
			buffer->discardPacket = buffer->currentMessageSize > MAX_PACKET_SIZE;

			buffer->currentMessageSize -= 4;
		}

		if(buffer->currentMessageSize != 0 && buffer->discardPacket) {
			buffer->currentMessageSize -= (uint32_t) inputStream->discard(buffer->currentMessageSize);
		} else if(buffer->currentMessageSize != 0 && inputStream->getAvailableBytes() >= buffer->currentMessageSize) {
			if(buffer->currentMessageSize+4 > buffer->bufferSize) {
				if(buffer->bufferSize)
					delete[] buffer->buffer;
				buffer->bufferSize = buffer->currentMessageSize+4;
				buffer->buffer = new uint8_t[buffer->currentMessageSize+4];
			}
			reinterpret_cast<TS_MESSAGE*>(buffer->buffer)->size = buffer->currentMessageSize+4;
			read(buffer->buffer + 4, buffer->currentMessageSize);
			dispatchPacket(reinterpret_cast<TS_MESSAGE*>(buffer->buffer));

			buffer->currentMessageSize = 0;
		}
	} while((buffer->currentMessageSize == 0 && inputStream->getAvailableBytes() >= 4) || (buffer->currentMessageSize != 0 && inputStream->getAvailableBytes() >= buffer->currentMessageSize));
}
