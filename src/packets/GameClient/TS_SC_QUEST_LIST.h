#ifndef PACKETS_TS_SC_QUEST_LIST_H
#define PACKETS_TS_SC_QUEST_LIST_H

#include "Packet/PacketDeclaration.h"

#define TS_QUEST_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, code) \
	simple_ (uint32_t, startID) \
	array_ (uint32_t, value, 6) \
	array_ (uint32_t, status, 6) \
	simple_ (uint8_t, progress) \
	simple_ (uint32_t, timeLimit)
CREATE_STRUCT(TS_QUEST_INFO);

#define TS_QUEST_PENDING_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, code) \
	simple_ (uint32_t, startID)
CREATE_STRUCT(TS_QUEST_PENDING);

#define TS_SC_QUEST_LIST_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	count_ (uint16_t, count_active, activeQuests) \
	dynarray_(TS_QUEST_INFO, activeQuests) \
	count_ (uint16_t, count_pending, pendingQuests) \
	dynarray_(TS_QUEST_PENDING, pendingQuests)

CREATE_PACKET(TS_SC_QUEST_LIST, 600);

#endif // PACKETS_TS_SC_QUEST_LIST_H
