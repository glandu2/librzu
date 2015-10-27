#ifndef PACKETS_TS_SC_INVENTORY_H
#define PACKETS_TS_SC_INVENTORY_H

#include "Packet/PacketDeclaration.h"

#define TS_ITEM_BASE_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, handle) \
	simple_ (int32_t, code) \
	simple_ (int64_t, uid) \
	simple_ (int64_t, count) \
	simple_ (int32_t, ethereal_durability) \
	simple_ (uint32_t, endurance) \
	simple_ (uint8_t, enhance) \
	simple_ (uint8_t, level) \
	simple_ (int32_t, flag) \
	array_  (int32_t, socket, 4) \
	array_  (uint32_t, awaken_option_value, 5) \
	array_  (int32_t, awaken_option_data, 5) \
	simple_ (int32_t, remain_time) \
	simple_ (uint8_t, elemental_effect_type) \
	simple_ (int32_t, elemental_effect_remain_time) \
	simple_ (int32_t, elemental_effect_attack_point) \
	simple_ (int32_t, elemental_effect_magic_point) \
	simple_ (int32_t, appearance_code)
CREATE_STRUCT(TS_ITEM_BASE_INFO);

#define TS_ITEM_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (TS_ITEM_BASE_INFO, base_info) \
	simple_ (int16_t , wear_position) \
	simple_ (uint32_t, own_summon_handle) \
	simple_ (int32_t , index)
CREATE_STRUCT(TS_ITEM_INFO);

#define TS_SC_INVENTORY_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	count_ (uint16_t, count, items) \
	dynarray_ (TS_ITEM_INFO, items)

CREATE_PACKET(TS_SC_INVENTORY, 207);

#endif // PACKETS_TS_SC_INVENTORY_H
