#pragma once

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AG_PCBANG_EXPIRE_WARNING : public TS_MESSAGE
{
	char account[61];
	uint32_t remain_minutes;

	static const uint16_t packetID = 20020;
};
#pragma pack(pop)

