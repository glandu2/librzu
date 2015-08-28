#ifndef PACKETS_TS_TIMESYNC_H
#define PACKETS_TS_TIMESYNC_H

#include "Packet/PacketBaseMessage.h"
#include "PacketEnums.h"

#pragma pack(push, 1)
struct TS_TIMESYNC : public TS_MESSAGE
{
	uint32_t time;
	static const int packetID = 2;
};
#pragma pack(pop)

#endif // PACKETS_TS_TIMESYNC_H
