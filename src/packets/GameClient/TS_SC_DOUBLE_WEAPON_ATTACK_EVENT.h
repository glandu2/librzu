#ifndef PACKETS_TS_SC_DOUBLE_WEAPON_ATTACK_EVENT_H
#define PACKETS_TS_SC_DOUBLE_WEAPON_ATTACK_EVENT_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_DOUBLE_WEAPON_ATTACK_EVENT_DEF(_) \
	_(simple)(uint32_t, attacker_handle) \
	_(simple)(uint32_t, target_handle) \
	_(simple)(int16_t, right_hand_damage) \
	_(simple)(int16_t, left_hand_damage) \
	_(simple)(uint16_t, attack_speed) \
	_(simple)(int8_t, hp_percentage) \
	_(simple)(uint16_t, flag) /* ATTACK_INFO__FLAG as 16 bits ? */ \
	_(array) (uint8_t, right_hand_elemental_damage, 7) \
	_(array) (uint8_t, left_hand_elemental_damage, 7) \

CREATE_PACKET(TS_SC_DOUBLE_WEAPON_ATTACK_EVENT, 103);

#endif // PACKETS_TS_SC_DOUBLE_WEAPON_ATTACK_EVENT_H
