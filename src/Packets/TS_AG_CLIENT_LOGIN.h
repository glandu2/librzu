#ifndef TS_AG_CLIENT_LOGIN_H
#define TS_AG_CLIENT_LOGIN_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AG_CLIENT_LOGIN : public TS_MESSAGE
{
	char account[61];
	int32_t nAccountID;
	uint16_t result;
	char nPCBangUser;
	int32_t nEventCode;
	int32_t nAge;
	uint32_t nContinuousPlayTime;
	uint32_t nContinuousLogoutTime;

	static const uint16_t packetID = 20011;
};
#pragma pack(pop)

#endif // TS_AG_CLIENT_LOGIN_H
