#ifndef TS_CA_VERSION_H
#define TS_CA_VERSION_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CA_VERSION : public TS_MESSAGE
{
	char szVersion[20];
	static const uint16_t packetID = 10001;
};
#pragma pack(pop)

#endif // TS_CA_VERSION_H
