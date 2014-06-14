#ifndef TS_CU_LOGIN_H
#define TS_CU_LOGIN_H

#include "Packets/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CU_LOGIN : public TS_MESSAGE
{
	uint32_t client_id;
	uint32_t account_id;
	uint32_t guild_id;
	uint32_t one_time_password;
	char raw_server_name[32];

	static const uint16_t packetID = 50005;
};
#pragma pack(pop)

#endif // TS_CU_LOGIN_H
