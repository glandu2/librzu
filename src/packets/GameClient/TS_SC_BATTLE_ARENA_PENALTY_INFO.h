#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BATTLE_ARENA_PENALTY_INFO_DEF(_) \
	_(simple)(ar_time_t, nBlockTime) \
	_(simple)(int32_t, nPenaltyCount) \
	_(simple)(ar_time_t, nPenaltyCountDecTime)

// Since EPIC_8_1
#define TS_SC_BATTLE_ARENA_PENALTY_INFO_ID(X) \
	X(4700, true)

CREATE_PACKET_VER_ID(TS_SC_BATTLE_ARENA_PENALTY_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BATTLE_ARENA_PENALTY_INFO_DEF

