#pragma once

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_GA_LOGOUT : public TS_MESSAGE
{
	static const uint16_t packetID = 60002;
};
#pragma pack(pop)

