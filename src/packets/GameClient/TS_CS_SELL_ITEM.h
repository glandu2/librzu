#ifndef PACKETS_TS_CS_SELL_ITEM_H
#define PACKETS_TS_CS_SELL_ITEM_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_SELL_ITEM_DEF(_) \
	_(simple)(uint32_t, handle) \
	_(simple)(uint16_t, sell_count)

CREATE_PACKET(TS_CS_SELL_ITEM, 252);

#endif // PACKETS_TS_CS_SELL_ITEM_H
