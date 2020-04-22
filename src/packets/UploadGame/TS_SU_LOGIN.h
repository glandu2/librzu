#pragma once

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_SU_LOGIN : public TS_MESSAGE
{
	char server_name[21];

	static const uint16_t packetID = 50001;
};
#pragma pack(pop)

