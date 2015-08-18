#ifndef PACKETS_TS_CS_VERSION_H
#define PACKETS_TS_CS_VERSION_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CS_VERSION : public TS_MESSAGE
{
	char szVersion[20];
	static const uint16_t packetID = 50;
	static const uint16_t packetIDv2 = 51;
};
#pragma pack(pop)

#endif // PACKETS_TS_CS_VERSION_H
