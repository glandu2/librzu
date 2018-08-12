#ifndef PACKETS_TS_AG_CLIENT_LOGIN_H
#define PACKETS_TS_AG_CLIENT_LOGIN_H

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AG_CLIENT_LOGIN : public TS_MESSAGE
{
	char account[61];
	uint32_t nAccountID;
	uint16_t result;
	char nPCBangUser;
	uint32_t nEventCode;
	uint32_t nAge;
	uint32_t nContinuousPlayTime;
	uint32_t nContinuousLogoutTime;

	static const uint16_t packetID = 20011;
};

struct TS_AG_CLIENT_LOGIN_EXTENDED : public TS_AG_CLIENT_LOGIN
{
	char ip[INET6_ADDRSTRLEN];
	uint32_t loginTime;

	static const uint16_t packetID = 60011;
};

#pragma pack(pop)

#endif // PACKETS_TS_AG_CLIENT_LOGIN_H
