#pragma once

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AG_ITEM_PURCHASED : public TS_MESSAGE
{
	char account[61];
	uint32_t nAccountID;

	static const uint16_t packetID = 30000;
};
#pragma pack(pop)

