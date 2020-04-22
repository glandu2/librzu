#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SKILL_INFO_DEF(_) \
	_(simple)(int32_t, skill_id) \
	_(simple)(int8_t, base_skill_level) \
	_(simple)(int8_t, current_skill_level) \
	_(simple)(ar_time_t, total_cool_time) \
	_(simple)(ar_time_t, remain_cool_time)

CREATE_STRUCT(TS_SKILL_INFO);
#undef TS_SKILL_INFO_DEF

#define TS_SC_SKILL_LIST_DEF(_) \
	_(simple)(ar_handle_t, target) \
	_(count)(uint16_t, skills) \
	_(simple)(int8_t, modification_type, version >= EPIC_4_1) \
	_(dynarray)(TS_SKILL_INFO, skills)

CREATE_PACKET(TS_SC_SKILL_LIST, 403);
#undef TS_SC_SKILL_LIST_DEF

// modification_type: if true then refresh

