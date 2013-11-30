#ifndef TS_CA_DISTRIBUTION_INFO_H
#define TS_CA_DISTRIBUTION_INFO_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CA_DISTRIBUTION_INFO : public TS_MESSAGE
{
	static const uint16_t packetID = 10026;
};
#pragma pack(pop)

#endif // TS_CA_DISTRIBUTION_INFO_H
