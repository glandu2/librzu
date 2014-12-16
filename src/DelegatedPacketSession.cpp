#include "DelegatedPacketSession.h"
#include "Packets/TS_SC_RESULT.h"

void DelegatedPacketSession::addPacketListener(uint16_t packetId, IListener* instance, CallbackFunction onPacketReceivedCallback) {
	packetListeners.add(packetId, instance, onPacketReceivedCallback);
}

void DelegatedPacketSession::removePacketListener(uint16_t packetId, IListener* instance) {
	packetListeners.del(packetId, instance);
}

void DelegatedPacketSession::onPacketReceived(const TS_MESSAGE *packet) {
	if(packet->id != TS_SC_RESULT::packetID)
		DELEGATE_HASH_CALL(packetListeners, packet->id, this, packet);
	else
		DELEGATE_HASH_CALL(packetListeners, reinterpret_cast<const TS_SC_RESULT*>(packet)->request_msg_id, this, packet);
}
