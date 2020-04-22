#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BATTLE_ARENA_UPDATE_WAIT_USER_COUNT_DEF(_) \
	_(simple)(int32_t, nWaitUserCount)

// Since EPIC_8_1
CREATE_PACKET(TS_SC_BATTLE_ARENA_UPDATE_WAIT_USER_COUNT, 4703);
#undef TS_SC_BATTLE_ARENA_UPDATE_WAIT_USER_COUNT_DEF

