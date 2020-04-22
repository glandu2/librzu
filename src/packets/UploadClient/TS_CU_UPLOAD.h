#pragma once

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CU_UPLOAD : public TS_MESSAGE_WNA
{
	uint32_t file_length;
	unsigned char file_contents[0];

	static const uint16_t packetID = 50007;
};
#pragma pack(pop)

