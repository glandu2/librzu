#ifndef PACKETS_TS_SC_UPDATE_ITEM_COUNT_H
#define PACKETS_TS_SC_UPDATE_ITEM_COUNT_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_UPDATE_ITEM_COUNT_DEF(_) \
	_(simple)(uint32_t, item_handle) \
	_(simple)(int64_t, count)

CREATE_PACKET(TS_SC_UPDATE_ITEM_COUNT, 255);

#endif // PACKETS_TS_SC_UPDATE_ITEM_COUNT_H
