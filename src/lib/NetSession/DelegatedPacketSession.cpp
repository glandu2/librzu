#include "DelegatedPacketSession.h"
#include "GameClient/TS_SC_RESULT.h"

void DelegatedPacketSession::addPacketListener(uint16_t packetId,
                                               IListener* instance,
                                               CallbackFunction onPacketReceivedCallback) {
	packetListeners.add(packetId, instance, onPacketReceivedCallback);
}

void DelegatedPacketSession::removePacketListener(uint16_t packetId, IListener* instance) {
	packetListeners.del(packetId, instance);
}

EventChain<PacketSession> DelegatedPacketSession::onPacketReceived(const TS_MESSAGE* packet) {
	if(packet->id != TS_SC_RESULT::packetID) {
		DELEGATE_HASH_CALL(packetListeners, packet->id, this, packet);
	} else {
		TS_SC_RESULT resultMsg;
		bool deserializationOk = packet->process(resultMsg, EPIC_LATEST);
		if(deserializationOk) {
			DELEGATE_HASH_CALL(packetListeners, resultMsg.request_msg_id, this, packet);
		}
	}

	return PacketSession::onPacketReceived(packet);
}
