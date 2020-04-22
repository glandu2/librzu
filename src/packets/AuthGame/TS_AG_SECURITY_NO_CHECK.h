#pragma once

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
//since epic 6
struct TS_AG_SECURITY_NO_CHECK : public TS_MESSAGE
{
	char account[61];
	uint32_t mode;
	uint32_t result;

	static const uint16_t packetID = 40000;
};

struct TS_AG_SECURITY_NO_CHECK_EPIC5 : public TS_MESSAGE
{
	char account[61];
	uint32_t result;

	static const uint16_t packetID = 40000;
};
#pragma pack(pop)

