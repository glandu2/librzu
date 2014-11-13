#ifndef PACKETS_TS_GA_SECURITY_NO_CHECK_H
#define PACKETS_TS_GA_SECURITY_NO_CHECK_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_GA_SECURITY_NO_CHECK : public TS_MESSAGE
{
	char account[61];
	char security[19];
	int32_t mode;

	static const uint16_t packetID = 40001;
};
#pragma pack(pop)

#endif // PACKETS_TS_GA_SECURITY_NO_CHECK_H
