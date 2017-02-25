#ifndef PACKETS_TS_SC_ATTACK_EVENT_H
#define PACKETS_TS_SC_ATTACK_EVENT_H

#include "Packet/PacketDeclaration.h"

#define ATTACK_INFO_DEF(_) \
	_(def)(simple)(int32_t, damage) \
	  _(impl)(simple)(int32_t, damage, version >= EPIC_7_1) \
	  _(impl)(simple)(int16_t, damage, version < EPIC_7_1) \
	_(def)(simple)(int32_t, mp_damage) \
	  _(impl)(simple)(int32_t, mp_damage, version >= EPIC_7_1) \
	  _(impl)(simple)(int16_t, mp_damage, version < EPIC_7_1) \
	_(simple) (int8_t, flag) \
	_(def)(array)(int32_t, elemental_damage, 7) \
	  _(impl)(array)(int32_t, elemental_damage, 7, version >= EPIC_7_1) \
	  _(impl)(array)(uint16_t, elemental_damage, 7, version < EPIC_7_1) \
	_(simple) (uint32_t, target_hp) \
	_(def)(simple)(int32_t, target_mp) \
	  _(impl)(simple)(int32_t, target_mp, version >= EPIC_7_1) \
	  _(impl)(simple)(uint16_t, target_mp, version < EPIC_7_1) \
	_(def)(simple)(int32_t, attacker_damage) \
	  _(impl)(simple)(int32_t, attacker_damage, version >= EPIC_7_1) \
	  _(impl)(simple)(int16_t, attacker_damage, version < EPIC_7_1) \
	_(def)(simple)(int32_t, attacker_mp_damage) \
	  _(impl)(simple)(int32_t, attacker_mp_damage, version >= EPIC_7_1) \
	  _(impl)(simple)(int16_t, attacker_mp_damage, version < EPIC_7_1) \
	_(simple) (uint32_t, attacker_hp) \
	_(def)(simple)(int32_t, attacker_mp) \
	  _(impl)(simple)(int32_t, attacker_mp, version >= EPIC_7_1) \
	  _(impl)(simple)(uint16_t, attacker_mp, version < EPIC_7_1)

CREATE_STRUCT(ATTACK_INFO);

#define TS_SC_ATTACK_EVENT_DEF(_) \
	_(simple)   (uint32_t, attacker_handle) \
	_(simple)   (uint32_t, target_handle) \
	_(simple)   (uint16_t, attack_speed) \
	_(simple)   (uint16_t, attack_delay) \
	_(simple)   (uint8_t, attack_action) \
	_(simple)   (uint8_t, attack_flag) \
	_(count)    (uint8_t, attack) \
	_(dynarray) (ATTACK_INFO, attack)

CREATE_PACKET(TS_SC_ATTACK_EVENT, 101);

#endif // PACKETS_TS_SC_ATTACK_EVENT_H
