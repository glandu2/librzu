#ifndef PACKETS_TS_SC_WEAR_INFO_H
#define PACKETS_TS_SC_WEAR_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_WEAR_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, handle) \
	array_(def)(uint32_t, itemCode, 24) \
	array_(impl)(uint32_t, itemCode, 14, version < EPIC_4_1) \
	array_(impl)(uint32_t, itemCode, 24, version >= EPIC_4_1) \
	array_(def)(uint32_t, itemEnhance, 24) \
	array_(impl)(uint32_t, itemEnhance, 14, version < EPIC_4_1) \
	array_(impl)(uint32_t, itemEnhance, 24, version >= EPIC_4_1) \
	array_(def)(uint32_t, itemLevel, 24) \
	array_(impl)(uint32_t, itemLevel, 14, version < EPIC_4_1) \
	array_(impl)(uint32_t, itemLevel, 24, version >= EPIC_4_1) \
	array_(uint8_t, elemental_effect_type, 24, version >= EPIC_4_1) \
	array_(uint32_t, appearance_code, 24, version >= EPIC_8_1)

CREATE_PACKET(TS_SC_WEAR_INFO, 202);

#endif // PACKETS_TS_SC_WEAR_INFO_H
