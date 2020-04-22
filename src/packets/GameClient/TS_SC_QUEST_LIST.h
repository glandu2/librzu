#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_QUEST_INFO_DEF(_) \
	_(simple) (uint32_t, code) \
	_(simple) (uint32_t, startID) \
	_(array) (uint32_t, value, 6) \
	_(def)(array) (uint32_t, status, 6) \
	_(impl)(array)(uint32_t, status, 6, version >= EPIC_6_1) \
	_(impl)(array)(uint32_t, status, 3, version < EPIC_6_1) \
	_(simple) (uint8_t, progress, version >= EPIC_6_3) \
	_(simple) (ar_time_t, timeLimit, version >= EPIC_6_3)
CREATE_STRUCT(TS_QUEST_INFO);
#undef TS_QUEST_INFO_DEF

#define TS_QUEST_PENDING_DEF(_) \
	_(simple) (uint32_t, code) \
	_(simple) (uint32_t, startID)
CREATE_STRUCT(TS_QUEST_PENDING);
#undef TS_QUEST_PENDING_DEF

#define TS_SC_QUEST_LIST_DEF(_) \
	_(count) (uint16_t, activeQuests) \
	_(count) (uint16_t, pendingQuests, version >= EPIC_6_3) \
	_(dynarray)(TS_QUEST_INFO, activeQuests) \
	_(dynarray)(TS_QUEST_PENDING, pendingQuests, version >= EPIC_6_3)

CREATE_PACKET(TS_SC_QUEST_LIST, 600);
#undef TS_SC_QUEST_LIST_DEF

