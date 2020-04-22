#pragma once

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_SU_REQUEST_UPLOAD : public TS_MESSAGE
{
	uint32_t client_id;
	uint32_t account_id;
	uint32_t guild_sid;
	uint32_t one_time_password;
	uint8_t type;

	static const uint16_t packetID = 50003;
};
#pragma pack(pop)

