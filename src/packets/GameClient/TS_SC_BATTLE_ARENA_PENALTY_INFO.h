#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BATTLE_ARENA_PENALTY_INFO_DEF(_) \
	_(simple)(ar_time_t, nBlockTime) \
	_(simple)(int32_t, nPenaltyCount) \
	_(simple)(ar_time_t, nPenaltyCountDecTime)

// Since EPIC_8_1
CREATE_PACKET(TS_SC_BATTLE_ARENA_PENALTY_INFO, 4700);
#undef TS_SC_BATTLE_ARENA_PENALTY_INFO_DEF

