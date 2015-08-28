#ifndef PACKETS_TS_GA_CLIENT_LOGIN_H
#define PACKETS_TS_GA_CLIENT_LOGIN_H

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_GA_CLIENT_LOGIN : public TS_MESSAGE
{
	char account[61];
	uint64_t one_time_key;

	static const uint16_t packetID = 20010;
};
#pragma pack(pop)

#endif // PACKETS_TS_GA_CLIENT_LOGIN_H
