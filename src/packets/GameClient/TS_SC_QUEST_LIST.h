#ifndef PACKETS_TS_SC_QUEST_LIST_H
#define PACKETS_TS_SC_QUEST_LIST_H

#include "Packet/PacketDeclaration.h"

#define TS_QUEST_INFO_DEF(_) \
	_(simple) (uint32_t, code) \
	_(simple) (uint32_t, startID) \
	_(array) (uint32_t, value, 6) \
	_(array) (uint32_t, status, 6) \
	_(simple) (uint8_t, progress) \
	_(simple) (uint32_t, timeLimit)
CREATE_STRUCT(TS_QUEST_INFO);

#define TS_QUEST_PENDING_DEF(_) \
	_(simple) (uint32_t, code) \
	_(simple) (uint32_t, startID)
CREATE_STRUCT(TS_QUEST_PENDING);

#define TS_SC_QUEST_LIST_DEF(_) \
	_(count) (uint16_t, count_active, activeQuests) \
	_(dynarray)(TS_QUEST_INFO, activeQuests) \
	_(count) (uint16_t, count_pending, pendingQuests) \
	_(dynarray)(TS_QUEST_PENDING, pendingQuests)

CREATE_PACKET(TS_SC_QUEST_LIST, 600);

#endif // PACKETS_TS_SC_QUEST_LIST_H
