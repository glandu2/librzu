#ifndef PACKETS_TS_SC_ADDED_SKILL_LIST_H
#define PACKETS_TS_SC_ADDED_SKILL_LIST_H

#include "Packet/PacketDeclaration.h"

#define TS_ADDED_SKILL_LIST_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (uint32_t, skill_id) \
	simple_ (bool, restricted_to_type) \
	simple_ (uint8_t, added_skill_level)
CREATE_STRUCT(TS_ADDED_SKILL_LIST);

#define TS_SC_ADDED_SKILL_LIST_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_  (uint32_t, target) \
	count_   (uint16_t, count, skills) \
	dynarray_(TS_ADDED_SKILL_LIST, skills)

CREATE_PACKET(TS_SC_ADDED_SKILL_LIST, 404);

#endif // PACKETS_TS_SC_ADDED_SKILL_LIST_H
