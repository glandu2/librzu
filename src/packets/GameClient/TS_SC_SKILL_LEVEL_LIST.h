#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SKILL_LEVEL_INFO_DEF(_) \
	_(simple)(int32_t, skill_id) \
	_(simple)(int8_t, skill_level)

CREATE_STRUCT(TS_SKILL_LEVEL_INFO);
#undef TS_SKILL_LEVEL_INFO_DEF

#define TS_SC_SKILL_LEVEL_LIST_DEF(_) \
	_(count)(uint16_t, skill_levels) \
	_(dynarray)(TS_SKILL_LEVEL_INFO, skill_levels)

// Since EPIC_7_3
#define TS_SC_SKILL_LEVEL_LIST_ID(X) \
	X(451, version < EPIC_9_6_3) \
	X(1451, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_SKILL_LEVEL_LIST, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_SKILL_LEVEL_LIST_DEF

