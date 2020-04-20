#ifndef PACKETS_TS_CS_DECOMPOSE_H
#define PACKETS_TS_CS_DECOMPOSE_H

#include "Packet/PacketDeclaration.h"

#define TS_DECOMPOSE_ITEM_INFO_DEF(_) \
	_(simple)(ar_handle_t, items_handle) \
	_(simple)(uint32_t, count, version >= EPIC_8_2, 1)
CREATE_STRUCT(TS_DECOMPOSE_ITEM_INFO);
#undef TS_DECOMPOSE_ITEM_INFO_DEF

#define TS_CS_DECOMPOSE_DEF(_) \
	_(count)(uint32_t, items) \
	_(dynarray)(TS_DECOMPOSE_ITEM_INFO, items)

// Since EPIC_8_1
CREATE_PACKET(TS_CS_DECOMPOSE, 265);
#undef TS_CS_DECOMPOSE_DEF

#endif // PACKETS_TS_CS_DECOMPOSE_H
