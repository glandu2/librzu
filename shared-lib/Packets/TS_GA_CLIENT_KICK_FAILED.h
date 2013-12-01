#ifndef TS_GA_CLIENT_KICK_FAILED_H
#define TS_GA_CLIENT_KICK_FAILED_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_GA_CLIENT_KICK_FAILED : public TS_MESSAGE
{
	char account[61];

	static const uint16_t packetID = 20014;
};
#pragma pack(pop)


#endif // TS_GA_CLIENT_KICK_FAILED_H
