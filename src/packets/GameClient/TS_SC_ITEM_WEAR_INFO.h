#ifndef PACKETS_TS_SC_ITEM_WEAR_INFO_H
#define PACKETS_TS_SC_ITEM_WEAR_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_ITEM_WEAR_INFO_DEF(_) \
	_(simple)(uint32_t, item_handle) \
	_(simple)(uint16_t, wear_position) \
	_(simple)(uint32_t, target_handle) \
	_(simple)(int32_t, enhance) \
	_(simple)(int8_t, wear_item_elemental_type, version >= EPIC_4_1) \
	_(simple)(int32_t, wear_appearance_code, version >= EPIC_8_1)

CREATE_PACKET(TS_SC_ITEM_WEAR_INFO, 287);

#endif // PACKETS_TS_SC_ITEM_WEAR_INFO_H
