#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_BATTLE_ARENA_JOIN_QUEUE_DEF(_) \
	_(simple)(int32_t, nArenaID)

// Since EPIC_8_1
CREATE_PACKET(TS_CS_BATTLE_ARENA_JOIN_QUEUE, 4701);
#undef TS_CS_BATTLE_ARENA_JOIN_QUEUE_DEF

