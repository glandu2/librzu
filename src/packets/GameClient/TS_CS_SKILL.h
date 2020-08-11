#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_SKILL_DEF(_) \
	_(simple)(uint16_t, skill_id) \
	_(simple)(ar_handle_t, caster) \
	_(simple)(ar_handle_t, target) \
	_(simple)(float, x) \
	_(simple)(float, y) \
	_(simple)(float, z) \
	_(simple)(int8_t, layer) \
	_(simple)(int8_t, skill_level)

#define TS_CS_SKILL_ID(X) \
	X(400, version < EPIC_9_6_3) \
	X(1400, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_SKILL, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_SKILL_DEF

