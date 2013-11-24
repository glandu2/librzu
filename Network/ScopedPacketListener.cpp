#include "ScopedPacketListener.h"

ScopedPacketListener::ScopedPacketListener()
{
}

ScopedPacketListener::~ScopedPacketListener() {
	removeAll();
}

void ScopedPacketListener::addPacketListeners(std::list<PacketListenerInfo> packetListeners) {
	std::list<PacketListenerInfo>::const_iterator it, itEnd;

	for(it = packetListeners.cbegin(), itEnd = packetListeners.cend(); it != itEnd; ++it) {
		const PacketListenerInfo& packetListenerInfo = *it;
		packetListenerInfo.server->addPacketListener(packetListenerInfo.serverType,
													 packetListenerInfo.packetId,
													 packetListenerInfo.callback,
													 packetListenerInfo.instance);
	}

	this->packetListeners.splice(this->packetListeners.end(), packetListeners);
}

void ScopedPacketListener::removeAll() {
	std::list<PacketListenerInfo>::const_iterator it, itEnd;

	for(it = packetListeners.cbegin(), itEnd = packetListeners.cend(); it != itEnd; ++it) {
		const PacketListenerInfo& packetListenerInfo = *it;
		packetListenerInfo.server->removePacketListener(packetListenerInfo.serverType,
													 packetListenerInfo.packetId,
													 packetListenerInfo.instance);
	}
}
