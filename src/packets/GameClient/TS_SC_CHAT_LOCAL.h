#ifndef PACKETS_TS_SC_CHAT_LOCAL_H
#define PACKETS_TS_SC_CHAT_LOCAL_H

#include "Packet/PacketBaseMessage.h"
#include "PacketEnums.h"

#pragma pack(push, 1)
struct TS_SC_CHAT_LOCAL : public TS_MESSAGE_WNA
{
	unsigned int handle;
	unsigned char len;
	char type;
	char message[0];
	static const int packetID = 21;
};
#pragma pack(pop)

#endif // PACKETS_TS_SC_CHAT_LOCAL_H
