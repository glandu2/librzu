#pragma once

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_US_REQUEST_UPLOAD : public TS_MESSAGE
{
	uint16_t result;

	static const uint16_t packetID = 50004;
};
#pragma pack(pop)

