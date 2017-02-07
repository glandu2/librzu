#ifndef PACKETS_TS_SC_WEAR_INFO_H
#define PACKETS_TS_SC_WEAR_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_WEAR_INFO_DEF(_) \
	_(simple) (uint32_t, handle) \
	_(def)(array)(uint32_t, wear_info, 24) \
	_(impl)(array)(uint32_t, wear_info, 14, version < EPIC_4_1) \
	_(impl)(array)(uint32_t, wear_info, 24, version >= EPIC_4_1) \
	_(def)(array)(uint32_t, wear_item_enhance_info, 24) \
	_(impl)(array)(uint32_t, wear_item_enhance_info, 14, version < EPIC_4_1) \
	_(impl)(array)(uint32_t, wear_item_enhance_info, 24, version >= EPIC_4_1) \
	_(def)(array)(uint32_t, wear_item_level_info, 24) \
	_(impl)(array)(uint32_t, wear_item_level_info, 14, version < EPIC_4_1) \
	_(impl)(array)(uint32_t, wear_item_level_info, 24, version >= EPIC_4_1) \
	_(array)(uint8_t, wear_item_elemental_type, 24, version >= EPIC_6_1) \
	_(array)(uint32_t, wear_appearance_code, 24, version >= EPIC_8_1) \
	_(simple)(uint8_t, booster_code, version >= EPIC_9_3) \
	_(simple)(uint32_t, emblem_code, version >= EPIC_9_3) \
	_(simple)(uint8_t, unknown1, version >= EPIC_9_3) \
	_(simple)(uint16_t, unknown2, version >= EPIC_9_3)

CREATE_PACKET(TS_SC_WEAR_INFO, 202);

#endif // PACKETS_TS_SC_WEAR_INFO_H
