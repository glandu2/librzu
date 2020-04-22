#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BATTLE_ARENA_BATTLE_STATUS_DEF(_) \
	_(simple)(int32_t, nStatus)

// Since EPIC_8_1
CREATE_PACKET(TS_SC_BATTLE_ARENA_BATTLE_STATUS, 4711);
#undef TS_SC_BATTLE_ARENA_BATTLE_STATUS_DEF

