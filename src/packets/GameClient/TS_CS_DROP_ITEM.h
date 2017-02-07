#ifndef PACKETS_TS_CS_DROP_ITEM_H
#define PACKETS_TS_CS_DROP_ITEM_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_DROP_ITEM_DEF(_) \
	_(simple)(uint32_t, item_handle) \
	_(simple)(int32_t, count)

CREATE_PACKET(TS_CS_DROP_ITEM, 203);

#endif // PACKETS_TS_CS_DROP_ITEM_H
