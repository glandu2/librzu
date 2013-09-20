#ifndef TS_CS_ACCOUNT_WITH_AUTH_H
#define TS_CS_ACCOUNT_WITH_AUTH_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CS_ACCOUNT_WITH_AUTH : public TS_MESSAGE
{
	char account[61];
	int64_t one_time_key;

	static const uint16_t packetID = 2005;
};
#pragma pack(pop)

#endif // TS_CS_ACCOUNT_WITH_AUTH_H
