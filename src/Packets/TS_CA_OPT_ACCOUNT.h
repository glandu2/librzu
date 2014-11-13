#ifndef PACKETS_TS_CA_OPT_ACCOUNT_H
#define PACKETS_TS_CA_OPT_ACCOUNT_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CA_OTP_ACCOUNT : public TS_MESSAGE
{
	char account[61];
	char OTP[48];
	static const uint16_t packetID = 10011;
};
#pragma pack(pop)

#endif // PACKETS_TS_CA_OPT_ACCOUNT_H
