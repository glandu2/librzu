#ifndef TS_US_LOGIN_RESULT_H
#define TS_US_LOGIN_RESULT_H

#include "Packets/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_US_LOGIN_RESULT : public TS_MESSAGE
{
	uint16_t result;

	static const uint16_t packetID = 50002;
};
#pragma pack(pop)

#endif // TS_US_LOGIN_RESULT_H
