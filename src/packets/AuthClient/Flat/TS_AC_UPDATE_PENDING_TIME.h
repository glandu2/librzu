#ifndef PACKETS_TS_AC_UPDATE_PENDING_TIME_H
#define PACKETS_TS_AC_UPDATE_PENDING_TIME_H

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AC_UPDATE_PENDING_TIME : public TS_MESSAGE //Not used in 6.1 server ...
{
	uint32_t pending_time;
	static const uint16_t packetID = 10025;
};
#pragma pack(pop)

#endif // PACKETS_TS_AC_UPDATE_PENDING_TIME_H
