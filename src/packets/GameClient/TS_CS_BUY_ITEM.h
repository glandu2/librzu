#ifndef PACKETS_TS_CS_BUY_ITEM_H
#define PACKETS_TS_CS_BUY_ITEM_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_BUY_ITEM_DEF(_) \
	_(simple)(int32_t, item_code) \
	_(simple)(uint16_t, buy_count)

CREATE_PACKET(TS_CS_BUY_ITEM, 251);

#endif // PACKETS_TS_CS_BUY_ITEM_H
