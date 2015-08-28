#ifndef PACKETS_TS_AG_PCBANG_EXPIRE_WARNING_H
#define PACKETS_TS_AG_PCBANG_EXPIRE_WARNING_H

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AG_PCBANG_EXPIRE_WARNING : public TS_MESSAGE
{
	char account[61];
	uint32_t remain_minutes;
};
#pragma pack(pop)

#endif // PACKETS_TS_AG_PCBANG_EXPIRE_WARNING_H
