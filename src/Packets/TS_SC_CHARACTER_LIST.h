#ifndef PACKETS_TS_SC_CHARACTER_LIST_H
#define PACKETS_TS_SC_CHARACTER_LIST_H

#include "PacketDeclaration.h"
#include "PacketEnums.h"

#define LOBBY_CHARACTER_INFO_DEF(simple_, array_, dynarray_, count_) \
	simple_ (uint32_t, sex) \
	simple_ (uint32_t, race) \
	array_  (uint32_t, model_id, 5) \
	simple_ (uint32_t, hair_color_index, version >= EPIC_4_1, 0) \
	simple_ (uint32_t, hair_color_rgb, version >= EPIC_4_1, 0) \
	simple_ (uint32_t, hide_equip_flag, version >= EPIC_4_1, 0) \
	simple_ (uint32_t, texture_id, version >= EPIC_4_1, 0) \
	array_  (uint32_t, wear_info, 14) \
	array_  (uint32_t, wear_info_2, 10, version >= EPIC_4_1, {0}) \
	simple_ (uint32_t, level) \
	simple_ (uint32_t, job) \
	simple_ (uint32_t, job_level) \
	simple_ (uint32_t, exp_percentage) \
	simple_ (uint32_t, hp) \
	simple_ (uint32_t, mp) \
	simple_ (uint32_t, permission) \
	simple_ (uint8_t, is_banned) \
	array_  (char, name, 19) \
	simple_ (uint32_t, skin_color, version >= EPIC_4_1, 0) \
	array_  (char, szCreateTime, 30) \
	array_  (char, szDeleteTime, 30) \
	array_  (uint32_t, wear_item_enhance_info, 14) \
	array_  (uint32_t, wear_item_enhance_info_2, 10, version >= EPIC_4_1, {0}) \
	array_  (uint32_t, wear_item_level_info, 14) \
	array_  (uint32_t, wear_item_level_info_2, 10, version >= EPIC_4_1, {0}) \
	array_  (uint8_t, wear_item_elemental_type, 24, version >= EPIC_4_1, {0}) \
	array_  (uint32_t, wear_appearance_code, 24, version >= EPIC_8_1, {0}) \

CREATE_STRUCT(LOBBY_CHARACTER_INFO); // struct is 304 bytes long in epic2

#define TS_SC_CHARACTER_LIST_DEF(simple_, array_, dynarray_, count_) \
	simple_   (uint32_t, current_server_time) \
	simple_   (uint16_t, last_character_idx, version >= EPIC_4_1, 1) \
	count_    (uint16_t, count, characters) \
	dynarray_ (LOBBY_CHARACTER_INFO, characters)

CREATE_PACKET(TS_SC_CHARACTER_LIST, 2004);

#endif // PACKETS_TS_SC_CHARACTER_LIST_H
