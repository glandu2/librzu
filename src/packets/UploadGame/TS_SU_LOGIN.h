#ifndef PACKETS_TS_SU_LOGIN_H
#define PACKETS_TS_SU_LOGIN_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_SU_LOGIN : public TS_MESSAGE
{
	char server_name[21];

	static const uint16_t packetID = 50001;
};
#pragma pack(pop)

#endif // PACKETS_TS_SU_LOGIN_H
