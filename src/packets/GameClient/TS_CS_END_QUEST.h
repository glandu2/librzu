#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_END_QUEST_DEF(_) \
	_(simple)(int32_t, code) \
	_(simple)(int8_t, nOptionalReward)

// Since EPIC_7_3
CREATE_PACKET(TS_CS_END_QUEST, 605);
#undef TS_CS_END_QUEST_DEF

