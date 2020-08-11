#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_DOUBLE_WEAPON_ATTACK_EVENT_DEF(_) \
	_(simple)(ar_handle_t, attacker_handle) \
	_(simple)(ar_handle_t, target_handle) \
	_(simple)(int16_t, right_hand_damage) \
	_(simple)(int16_t, left_hand_damage) \
	_(simple)(uint16_t, attack_speed) \
	_(simple)(int8_t, hp_percentage) \
	_(simple)(uint16_t, flag) /* ATTACK_INFO__FLAG as 16 bits ? */ \
	_(array) (uint8_t, right_hand_elemental_damage, 7) \
	_(array) (uint8_t, left_hand_elemental_damage, 7) \

#define TS_SC_DOUBLE_WEAPON_ATTACK_EVENT_ID(X) \
	X(103, version < EPIC_9_6_3) \
	X(1103, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_DOUBLE_WEAPON_ATTACK_EVENT, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_DOUBLE_WEAPON_ATTACK_EVENT_DEF

