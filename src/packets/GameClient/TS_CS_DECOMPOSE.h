#ifndef PACKETS_TS_CS_DECOMPOSE_H
#define PACKETS_TS_CS_DECOMPOSE_H

#include "Packet/PacketDeclaration.h"

#define TS_DECOMPOSE_ITEM_INFO_DEF(_) \
	_(simple)(uint32_t, items_handle) \
	_(simple)(uint32_t, count, version >= EPIC_8_3, 1)
CREATE_STRUCT(TS_DECOMPOSE_ITEM_INFO);

#define TS_CS_DECOMPOSE_DEF(_) \
	_(count)(uint32_t, items) \
	_(dynarray)(TS_DECOMPOSE_ITEM_INFO, items)

// Since EPIC_8_1
CREATE_PACKET(TS_CS_DECOMPOSE, 265);

#endif // PACKETS_TS_CS_DECOMPOSE_H
