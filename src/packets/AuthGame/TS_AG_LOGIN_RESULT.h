#pragma once

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AG_LOGIN_RESULT : public TS_MESSAGE
{
	uint16_t result;

	static const uint16_t packetID = 20002;
};
#pragma pack(pop)

