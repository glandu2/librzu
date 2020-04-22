#pragma once

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CA_SELECT_SERVER : public TS_MESSAGE
{
	uint16_t server_idx;
	static const uint16_t packetID = 10023;
};
#pragma pack(pop)

