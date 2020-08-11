#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_ADDED_SKILL_LIST_DEF(_) \
	_(simple) (uint32_t, skill_id) \
	_(simple) (bool, restricted_to_type) \
	_(simple) (uint8_t, added_skill_level)
CREATE_STRUCT(TS_ADDED_SKILL_LIST);
#undef TS_ADDED_SKILL_LIST_DEF

#define TS_SC_ADDED_SKILL_LIST_DEF(_) \
	_(simple)  (ar_handle_t, target) \
	_(count)   (uint16_t, skills) \
	_(dynarray)(TS_ADDED_SKILL_LIST, skills)

#define TS_SC_ADDED_SKILL_LIST_ID(X) \
	X(404, version < EPIC_9_6_3) \
	X(1404, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_ADDED_SKILL_LIST, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_ADDED_SKILL_LIST_DEF

