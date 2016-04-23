#ifndef PACKETS_TS_SC_ATTACK_EVENT_H
#define PACKETS_TS_SC_ATTACK_EVENT_H

#include "Packet/PacketDeclaration.h"

#define ATTACK_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (int32_t, damage) \
	simple_ (int32_t, mp_damage) \
	simple_ (int8_t, flag) \
	array_  (int32_t, elemental_damage, 7) \
	simple_ (int32_t, target_hp) \
	simple_ (int32_t, target_mp) \
	simple_ (int32_t, attacker_damage) \
	simple_ (int32_t, attacker_mp_damage) \
	simple_ (int32_t, attacker_hp) \
	simple_ (int32_t, attacker_mp)

CREATE_STRUCT(ATTACK_INFO);

#define TS_SC_ATTACK_EVENT_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_   (uint32_t, attacker_handle) \
	simple_   (uint32_t, target_handle) \
	simple_   (uint16_t, attack_speed) \
	simple_   (uint16_t, attack_delay) \
	simple_   (uint8_t, attack_action) \
	simple_   (uint8_t, attack_flag) \
	count_    (uint8_t, count, attack) \
	dynarray_ (ATTACK_INFO, attack)

CREATE_PACKET(TS_SC_ATTACK_EVENT, 101);

#endif // PACKETS_TS_SC_ATTACK_EVENT_H
