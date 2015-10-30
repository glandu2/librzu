#ifndef PACKETS_TS_SC_ITEM_WEAR_INFO_H
#define PACKETS_TS_SC_ITEM_WEAR_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_ITEM_WEAR_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_(uint32_t, item_handle) \
	simple_(uint16_t, wear_position) \
	simple_(uint32_t, target_handle) \
	simple_(int32_t, enhance) \
	simple_(int8_t, wear_item_elemental_type, version >= EPIC_4_1) \
	simple_(int32_t, wear_appearance_code, version >= EPIC_8_1)

CREATE_PACKET(TS_SC_ITEM_WEAR_INFO, 287);

#endif // PACKETS_TS_SC_ITEM_WEAR_INFO_H
