#ifndef PACKETS_TS_GA_CLIENT_LOGOUT_H
#define PACKETS_TS_GA_CLIENT_LOGOUT_H

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_GA_CLIENT_LOGOUT : public TS_MESSAGE
{
	char account[61];
	uint32_t nContinuousPlayTime;

	static const uint16_t packetID = 20012;
};
#pragma pack(pop)

#endif // PACKETS_TS_GA_CLIENT_LOGOUT_H
