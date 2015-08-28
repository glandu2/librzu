#ifndef PACKETS_TS_CS_UPDATE_H
#define PACKETS_TS_CS_UPDATE_H

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CS_UPDATE : public TS_MESSAGE
{
	unsigned int handle;
	static const int packetID = 503;
};
#pragma pack(pop)

#endif // PACKETS_TS_CS_UPDATE_H
