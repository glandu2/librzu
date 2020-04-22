#pragma once

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CA_VERSION : public TS_MESSAGE
{
	char szVersion[20];
	static const uint16_t packetID = 10001;
};
#pragma pack(pop)

