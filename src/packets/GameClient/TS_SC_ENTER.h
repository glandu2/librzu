#ifndef PACKETS_TS_SC_ENTER_H
#define PACKETS_TS_SC_ENTER_H

#include "Packet/PacketDeclaration.h"
#include "Packet/EncodedInt.h"
#include "Packet/EncodingRandomized.h"
#include "Packet/EncodingScrambled.h"

enum TS_SC_ENTER__OBJ_TYPE : uint8_t {
	EOT_Player,
	EOT_NPC,
	EOT_Item,
	EOT_Monster,
	EOT_Summon,
	EOT_Skill,
	EOT_FieldProp,
	EOT_Pet
};

#define TS_SC_ENTER__ITEM_PICK_UP_ORDER_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, drop_time) \
	array_  (uint32_t, hPlayer, 3) \
	array_  (int32_t, nPartyID, 3)
CREATE_STRUCT(TS_SC_ENTER__ITEM_PICK_UP_ORDER);

#define TS_SC_ENTER__ITEM_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (EncodedInt<EncodingRandomized>, code) \
	simple_ (uint64_t, count) \
	simple_ (TS_SC_ENTER__ITEM_PICK_UP_ORDER, pick_up_order)
CREATE_STRUCT(TS_SC_ENTER__ITEM_INFO);

#define TS_SC_ENTER__SKILL_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, casterHandle) \
	simple_ (uint32_t, startTime) \
	simple_ (uint32_t, skillId)
CREATE_STRUCT(TS_SC_ENTER__SKILL_INFO);

#define TS_SC_ENTER__FIELD_PROP_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, prop_id) \
	simple_ (float, fZOffset) \
	simple_ (float, fRotateX) \
	simple_ (float, fRotateY) \
	simple_ (float, fRotateZ) \
	simple_ (float, fScaleX) \
	simple_ (float, fScaleY) \
	simple_ (float, fScaleZ) \
	simple_ (bool, bLockHeight) \
	simple_ (float, fLockHeight)
CREATE_STRUCT(TS_SC_ENTER__FIELD_PROP_INFO);

#define TS_SC_ENTER__CREATURE_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, status) \
	simple_ (float, face_direction) \
	simple_ (int32_t, hp) \
	simple_ (int32_t, max_hp) \
	simple_ (int32_t, mp) \
	simple_ (int32_t, max_mp) \
	simple_ (int32_t, level) \
	simple_ (uint8_t, race) \
	simple_ (uint32_t, skin_color) \
	simple_ (bool, is_first_enter) \
	simple_ (int32_t, energy)
CREATE_STRUCT(TS_SC_ENTER__CREATURE_INFO);

#define TS_SC_ENTER__MONSTER_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (TS_SC_ENTER__CREATURE_INFO, creatureInfo) \
	simple_ (EncodedInt<EncodingScrambled>, monster_id) \
	simple_ (bool, is_tamed)
CREATE_STRUCT(TS_SC_ENTER__MONSTER_INFO);

#define TS_SC_ENTER__SUMMON_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (TS_SC_ENTER__CREATURE_INFO, creatureInfo) \
	simple_ (uint32_t, master_handle) \
	simple_ (EncodedInt<EncodingRandomized>, summon_code) \
	string_ (szName, 19) \
	simple_ (uint8_t, enhance)
CREATE_STRUCT(TS_SC_ENTER__SUMMON_INFO);

#define TS_SC_ENTER__NPC_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (TS_SC_ENTER__CREATURE_INFO, creatureInfo) \
	simple_ (EncodedInt<EncodingRandomized>, npc_id)
CREATE_STRUCT(TS_SC_ENTER__NPC_INFO);

#define TS_SC_ENTER__PLAYER_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (TS_SC_ENTER__CREATURE_INFO, creatureInfo) \
	simple_ (uint8_t, sex) \
	simple_ (uint32_t, faceId) \
	simple_ (uint32_t, faceTextureId) \
	simple_ (uint32_t, hairId) \
	simple_ (uint32_t, hairColorIndex) \
	simple_ (uint32_t, hairColorRGB) \
	simple_ (uint32_t, hideEquipFlag) \
	string_ (szName, 19) \
	simple_ (uint16_t, job_id) \
	simple_ (uint32_t, ride_handle) \
	simple_ (uint32_t, guild_id) \
	simple_ (uint64_t, unknown, version >= EPIC_9_2)
CREATE_STRUCT(TS_SC_ENTER__PLAYER_INFO);

#define TS_SC_ENTER__PET_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (TS_SC_ENTER__CREATURE_INFO, creatureInfo) \
	simple_ (uint32_t, master_handle) \
	simple_ (EncodedInt<EncodingRandomized>, pet_code) \
	string_ (szName, 19)
CREATE_STRUCT(TS_SC_ENTER__PET_INFO);

#define TS_SC_ENTER_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint8_t, type) /* 0 = static object, 1 = movable object, 2 = client object (ArObject::ObjectType) */ \
	simple_ (uint32_t, handle) \
	simple_ (float, x) \
	simple_ (float, y) \
	simple_ (float, z) \
	simple_ (uint8_t, layer) \
	simple_ (TS_SC_ENTER__OBJ_TYPE, objType) \
	simple_ (TS_SC_ENTER__PLAYER_INFO    , playerInfo   , objType == EOT_Player) \
	simple_ (TS_SC_ENTER__NPC_INFO       , npcInfo      , objType == EOT_NPC) \
	simple_ (TS_SC_ENTER__ITEM_INFO      , itemInfo     , objType == EOT_Item) \
	simple_ (TS_SC_ENTER__MONSTER_INFO   , monsterInfo  , objType == EOT_Monster) \
	simple_ (TS_SC_ENTER__SUMMON_INFO    , summonInfo   , objType == EOT_Summon) \
	simple_ (TS_SC_ENTER__SKILL_INFO     , skillInfo    , objType == EOT_Skill) \
	simple_ (TS_SC_ENTER__FIELD_PROP_INFO, fieldPropInfo, objType == EOT_FieldProp) \
	simple_ (TS_SC_ENTER__PET_INFO       , petInfo      , objType == EOT_Pet)

#define TS_SC_ENTER_ID(X) \
	X(3, version < EPIC_9_2) \
	X(63, version >= EPIC_9_2)

CREATE_PACKET_VER_ID(TS_SC_ENTER);

#endif // PACKETS_TS_SC_ENTER_H
