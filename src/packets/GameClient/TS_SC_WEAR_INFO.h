#ifndef PACKETS_TS_SC_WEAR_INFO_H
#define PACKETS_TS_SC_WEAR_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_WEAR_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, handle) \
	array_(def)(uint32_t, wear_info, 24) \
	array_(impl)(uint32_t, wear_info, 14, version < EPIC_4_1) \
	array_(impl)(uint32_t, wear_info, 24, version >= EPIC_4_1) \
	array_(def)(uint32_t, wear_item_enhance_info, 24) \
	array_(impl)(uint32_t, wear_item_enhance_info, 14, version < EPIC_4_1) \
	array_(impl)(uint32_t, wear_item_enhance_info, 24, version >= EPIC_4_1) \
	array_(def)(uint32_t, wear_item_level_info, 24) \
	array_(impl)(uint32_t, wear_item_level_info, 14, version < EPIC_4_1) \
	array_(impl)(uint32_t, wear_item_level_info, 24, version >= EPIC_4_1) \
	array_(uint8_t, wear_item_elemental_type, 24, version >= EPIC_4_1) \
	array_(uint32_t, wear_appearance_code, 24, version >= EPIC_8_1)

CREATE_PACKET(TS_SC_WEAR_INFO, 202);

#endif // PACKETS_TS_SC_WEAR_INFO_H
