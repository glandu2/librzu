#ifndef DELEGATEDPACKETSESSION_H
#define DELEGATEDPACKETSESSION_H

#include "PacketSession.h"
#include "IDelegate.h"

class LIB_EXTERN DelegatedPacketSession : public PacketSession
{
	DECLARE_CLASS(DelegatedPacketSession)
public:
	typedef void (*CallbackFunction)(IListener* instance, PacketSession* server, const TS_MESSAGE* packetData);

public:
	void addPacketListener(uint16_t packetId, IListener* instance, CallbackFunction onPacketReceivedCallback);
	void removePacketListener(uint16_t packetId, IListener* instance);

protected:
	void onPacketReceived(const TS_MESSAGE* packet);

private:
	IDelegateHash<uint16_t, CallbackFunction> packetListeners;
};

#endif // DELEGATEDPACKETSESSION_H
