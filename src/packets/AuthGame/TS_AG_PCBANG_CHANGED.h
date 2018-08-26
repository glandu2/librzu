#ifndef PACKETS_TS_AG_PCBANG_EXPIRE_H
#define PACKETS_TS_AG_PCBANG_EXPIRE_H

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AG_PCBANG_EXPIRE : public TS_MESSAGE
{
	char account[61];
	int nPCBangMode;

	static const uint16_t packetID = 20022;
};
#pragma pack(pop)

#endif // PACKETS_TS_AG_PCBANG_EXPIRE_H
