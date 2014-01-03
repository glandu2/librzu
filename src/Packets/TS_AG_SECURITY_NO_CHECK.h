#ifndef TS_AG_SECURITY_NO_CHECK_H
#define TS_AG_SECURITY_NO_CHECK_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AG_SECURITY_NO_CHECK : public TS_MESSAGE
{
	char account[61];
	uint32_t mode;
	uint32_t result; //not on 5.2 server

	static const uint16_t packetID = 40000;
};
#pragma pack(pop)

#endif // TS_AG_SECURITY_NO_CHECK_H
