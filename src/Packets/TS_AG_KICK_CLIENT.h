#ifndef PACKETS_TS_AG_KICK_CLIENT_H
#define PACKETS_TS_AG_KICK_CLIENT_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AG_KICK_CLIENT : public TS_MESSAGE
{
	char account[61];

	enum KickType : char
	{
		KICK_TYPE_ANOTHER_LOGIN = 0x0,  //Gameguard kick ?
		KICK_TYPE_DUPLICATED_LOGIN = 0x1,
		KICK_TYPE_BILLING_EXPIRED = 0x2,
		KICK_TYPE_GAME_ADDICTION = 0x3
	} kick_type; //not in 5.2 gs

	static const uint16_t packetID = 20013;
};
#pragma pack(pop)

#endif // PACKETS_TS_AG_KICK_CLIENT_H
