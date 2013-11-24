#ifndef SCOPEDPACKETLISTENER_H
#define SCOPEDPACKETLISTENER_H

#include "Server.h"
#include <list>

class ScopedPacketListener
{
public:
	struct PacketListenerInfo {
		Server *server;
		Server::ServerType serverType;
		uint16_t packetId;
		void *instance;
		Server::CallbackFunction callback;
	};

	ScopedPacketListener();
	~ScopedPacketListener();

	void addPacketListeners(std::list<PacketListenerInfo> packetListeners);
	void removeAll();

private:
	std::list<PacketListenerInfo> packetListeners;
};

#endif // SCOPEDPACKETLISTENER_H
