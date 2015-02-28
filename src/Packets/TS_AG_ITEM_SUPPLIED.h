#ifndef PACKETS_TS_AG_ITEM_SUPPLIED_H
#define PACKETS_TS_AG_ITEM_SUPPLIED_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AG_ITEM_SUPPLIED : public TS_MESSAGE
{
	char account[61];

	static const uint16_t packetID = 30001;
};
#pragma pack(pop)

#endif // PACKETS_TS_AG_ITEM_SUPPLIED_H
