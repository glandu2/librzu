#ifndef PACKETS_TS_SC_SKILL_H
#define PACKETS_TS_SC_SKILL_H

#include "Packet/PacketDeclaration.h"

enum TS_SKILL__TYPE : uint8_t
{
	ST_Fire = 0,
	ST_Casting = 1,
	ST_CastingUpdate = 2,
	ST_Cancel = 3,
	ST_RegionFire = 4,
	ST_Complete = 5
};

enum TS_SKILL__HIT_TYPE : uint8_t
{
	SHT_DAMAGE = 0,
	SHT_MAGIC_DAMAGE = 1,
	SHT_DAMAGE_WITH_KNOCK_BACK = 2,
	SHT_RESULT = 10,
	SHT_ADD_HP = 20,
	SHT_ADD_MP = 21,
	SHT_ADD_HP_MP_SP = 22,
	SHT_REBIRTH = 23,
	SHT_RUSH = 30,
	SHT_CHAIN_DAMAGE = 40,
	SHT_CHAIN_MAGIC_DAMAGE = 41,
	SHT_CHAIN_HEAL = 42,
	SHT_NOT_USE = 100
};

enum TS_SKILL__DAMAGE_TYPE : uint8_t
{
	SDT_TYPE_NONE = 0x0,
	SDT_TYPE_FIRE = 0x1,
	SDT_TYPE_WATER = 0x2,
	SDT_TYPE_WIND = 0x3,
	SDT_TYPE_EARTH = 0x4,
	SDT_TYPE_LIGHT = 0x5,
	SDT_TYPE_DARK = 0x6,
	SDT_TYPE_COUNT = 0x7
};

#define TS_SC_SKILL__HIT_DAMAGE_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (int32_t, target_hp) \
	simple_ (TS_SKILL__DAMAGE_TYPE, damage_type) \
	simple_ (int32_t, damage) \
	simple_ (int32_t, flag) \
	array_  (uint16_t, elemental_damage, 7)
CREATE_STRUCT(TS_SC_SKILL__HIT_DAMAGE_INFO);

#define TS_SC_SKILL__HIT_DAMAGE_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (TS_SC_SKILL__HIT_DAMAGE_INFO, damage) \
	pad_    (14)
CREATE_STRUCT(TS_SC_SKILL__HIT_DAMAGE);

#define TS_SC_SKILL__HIT_DAMAGE_WITH_KNOCKBACK_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (TS_SC_SKILL__HIT_DAMAGE_INFO, damage) \
	simple_ (float, x) \
	simple_ (float, y) \
	simple_ (int16_t, speed) \
	simple_ (uint32_t, knock_back_time)
CREATE_STRUCT(TS_SC_SKILL__HIT_DAMAGE_WITH_KNOCKBACK);

#define TS_SC_SKILL__HIT_RESULT_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (bool, bResult) \
	simple_ (int32_t, success_type) \
	pad_    (35)
CREATE_STRUCT(TS_SC_SKILL__HIT_RESULT);

#define TS_SC_SKILL__HIT_ADD_STAT_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (int32_t, target_stat) \
	simple_ (int32_t, nIncStat) \
	pad_    (32)
CREATE_STRUCT(TS_SC_SKILL__HIT_ADD_STAT);

#define TS_SC_SKILL__HIT_ADDHPMPSP_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (int32_t, target_hp) \
	simple_ (int32_t, nIncHP) \
	simple_ (int32_t, nIncMP) \
	simple_ (int32_t, nIncSP) \
	simple_ (int32_t, target_mp) \
	pad_    (22)
CREATE_STRUCT(TS_SC_SKILL__HIT_ADDHPMPSP);

#define TS_SC_SKILL__HIT_REBIRTH_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (int32_t, target_hp) \
	simple_ (int32_t, nIncHP) \
	simple_ (int32_t, nIncMP) \
	simple_ (int32_t, nRecoveryEXP) \
	simple_ (int32_t, target_mp) \
	pad_    (22)
CREATE_STRUCT(TS_SC_SKILL__HIT_REBIRTH);

#define TS_SC_SKILL__HIT_RUSH_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (bool, bResult) \
	simple_ (float, x) \
	simple_ (float, y) \
	simple_ (float, face) \
	simple_ (int8_t, speed) \
	pad_    (26)
CREATE_STRUCT(TS_SC_SKILL__HIT_RUSH);

#define TS_SC_SKILL__HIT_CHAIN_DAMAGE_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (TS_SC_SKILL__HIT_DAMAGE_INFO, damage) \
	simple_ (uint32_t, hFrom) \
	pad_    (9)
CREATE_STRUCT(TS_SC_SKILL__HIT_CHAIN_DAMAGE);

#define TS_SC_SKILL__HIT_CHAIN_HEAL_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (int32_t, target_hp) \
	simple_ (int32_t, nIncHP) \
	simple_ (uint32_t, hFrom) \
	pad_    (28)
CREATE_STRUCT(TS_SC_SKILL__HIT_CHAIN_HEAL);

#define TS_SC_SKILL__HIT_DETAILS_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (TS_SKILL__HIT_TYPE, type) \
	simple_ (uint32_t, hTarget) \
	simple_ (TS_SC_SKILL__HIT_DAMAGE               , hitDamage             , type == SHT_DAMAGE || type == SHT_MAGIC_DAMAGE) \
	simple_ (TS_SC_SKILL__HIT_DAMAGE_WITH_KNOCKBACK, hitDamageWithKnockBack, type == SHT_DAMAGE_WITH_KNOCK_BACK) \
	simple_ (TS_SC_SKILL__HIT_RESULT               , hitResult             , type == SHT_RESULT) \
	simple_ (TS_SC_SKILL__HIT_ADD_STAT             , hitAddStat            , type == SHT_ADD_HP || type == SHT_ADD_MP) \
	simple_ (TS_SC_SKILL__HIT_ADDHPMPSP            , hitAddHPMPSP          , type == SHT_ADD_HP_MP_SP) \
	simple_ (TS_SC_SKILL__HIT_REBIRTH              , hitRebirth            , type == SHT_REBIRTH) \
	simple_ (TS_SC_SKILL__HIT_RUSH                 , hitRush               , type == SHT_RUSH) \
	simple_ (TS_SC_SKILL__HIT_CHAIN_DAMAGE         , hitChainDamage        , type == SHT_CHAIN_DAMAGE || type == SHT_CHAIN_MAGIC_DAMAGE) \
	simple_ (TS_SC_SKILL__HIT_CHAIN_HEAL           , hitAddHP              , type == SHT_CHAIN_HEAL)
CREATE_STRUCT(TS_SC_SKILL__HIT_DETAILS);

#define TS_SC_SKILL__FIRE_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (bool, bMultiple) \
	simple_ (float, range) \
	simple_ (int8_t, target_count) \
	simple_ (int8_t, fire_count) \
	count_  (uint16_t, count, hits) \
	dynarray_(TS_SC_SKILL__HIT_DETAILS, hits)
CREATE_STRUCT(TS_SC_SKILL__FIRE);

#define TS_SC_SKILL__CAST_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (uint32_t, tm) \
	simple_ (uint16_t, nErrorCode) \
	pad_    (3) /* padding to match fire size */
CREATE_STRUCT(TS_SC_SKILL__CAST);

#define TS_SC_SKILL_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (uint16_t, skill_id) \
	simple_ (uint8_t, skill_level) \
	simple_ (uint32_t, caster) \
	simple_ (uint32_t, target) \
	simple_ (float, x) \
	simple_ (float, y) \
	simple_ (float, z) \
	simple_ (uint8_t, layer) \
	simple_ (TS_SKILL__TYPE, type) \
	simple_ (int32_t, hp_cost) \
	simple_ (int32_t, mp_cost) \
	simple_ (int32_t, caster_hp) \
	simple_ (int32_t, caster_mp) \
	simple_ (TS_SC_SKILL__FIRE , fire   , type == ST_Fire || type == ST_RegionFire) \
	simple_ (TS_SC_SKILL__CAST , casting, type == ST_Casting || type == ST_CastingUpdate) \
	pad_    (9, type != ST_Fire && type != ST_RegionFire && type != ST_Casting && type != ST_CastingUpdate) /* padding to match fire size */

CREATE_PACKET(TS_SC_SKILL, 401);

#endif // PACKETS_TS_SC_ENTER_H
