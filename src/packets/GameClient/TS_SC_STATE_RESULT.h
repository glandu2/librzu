#pragma once

#include "Packet/PacketDeclaration.h"

enum TS_STATE_RESULT_TYPE : uint16_t {
	SRT_Damage = 1,
	SRT_MagicDamage = 2,
	SRT_SPDamage = 3,
	SRT_Heal = 4,
	SRT_MagicHeal = 5,
	SRT_SPHeal = 6
};

#define TS_SC_STATE_RESULT_DEF(_) \
	_(simple)(ar_handle_t, caster_handle) \
	_(simple)(ar_handle_t, target_handle) \
	_(simple)(int32_t, code) \
	_(simple)(uint16_t, level) \
	_(simple)(TS_STATE_RESULT_TYPE, result_type) \
	_(simple)(int32_t, value) \
	_(simple)(int32_t, target_value) \
	_(simple)(bool, final) \
	_(simple)(int32_t, total_amount)

#define TS_SC_STATE_RESULT_ID(X) \
	X(406, version < EPIC_9_6_3) \
	X(1406, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_STATE_RESULT, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_STATE_RESULT_DEF

