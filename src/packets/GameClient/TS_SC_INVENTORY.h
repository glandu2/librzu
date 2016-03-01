#ifndef PACKETS_TS_SC_INVENTORY_H
#define PACKETS_TS_SC_INVENTORY_H

#include "Packet/PacketDeclaration.h"

#define TS_ITEM_BASE_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, handle) \
	simple_ (int32_t, code) \
	simple_ (int64_t, uid) \
	simple_(def)(int64_t, count) \
	simple_(impl)(int64_t, count, version >= EPIC_4_1) \
	simple_(impl)(int32_t, count, version < EPIC_4_1) \
	simple_ (int32_t, ethereal_durability, version >= EPIC_6_2) \
	simple_(def) (uint32_t, endurance) \
	simple_(impl)(uint32_t, endurance, version >= EPIC_4_1) \
	simple_(impl)(uint16_t, endurance, version < EPIC_4_1) \
	simple_ (uint8_t, enhance) \
	simple_ (uint8_t, level) \
	simple_ (int32_t, flag) \
	array_  (int32_t, socket, 4) \
	array_  (int32_t, dummy_socket, 2, version < EPIC_7_1, {0}) \
	array_  (uint32_t, awaken_option_value, 5, version >= EPIC_8_1) \
	array_  (int32_t, awaken_option_data, 5, version >= EPIC_8_1) \
	simple_ (int32_t, remain_time) \
	simple_ (uint8_t, elemental_effect_type, version >= EPIC_6_1) \
	simple_ (int32_t, elemental_effect_remain_time, version >= EPIC_6_1) \
	simple_ (int32_t, elemental_effect_attack_point, version >= EPIC_6_1) \
	simple_ (int32_t, elemental_effect_magic_point, version >= EPIC_6_1) \
	simple_ (int32_t, appearance_code, version >= EPIC_8_1) \
	simple_ (int16_t, unknown2, version >= EPIC_9_2) \
	array_  (int32_t, unknown1, 51, version >= EPIC_9_1)
CREATE_STRUCT(TS_ITEM_BASE_INFO);

#define TS_ITEM_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (TS_ITEM_BASE_INFO, base_info) \
	simple_ (int16_t , wear_position) \
	simple_ (uint32_t, own_summon_handle, version >= EPIC_5_1) \
	simple_ (int32_t , index, version >= EPIC_5_1)
CREATE_STRUCT(TS_ITEM_INFO);

#define TS_SC_INVENTORY_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	count_ (uint16_t, count, items) \
	dynarray_ (TS_ITEM_INFO, items)

CREATE_PACKET(TS_SC_INVENTORY, 207);

#endif // PACKETS_TS_SC_INVENTORY_H
