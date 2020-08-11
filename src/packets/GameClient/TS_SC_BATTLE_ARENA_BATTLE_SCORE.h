#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_BATTLE_ARENA_TEAM_SCORE_DEF(_) \
	_(simple)(int16_t, nTotalKillCount) \
	_(simple)(int16_t, nTotalDeathCount) \
	_(simple)(int16_t, nScore)

CREATE_STRUCT(TS_BATTLE_ARENA_TEAM_SCORE);
#undef TS_BATTLE_ARENA_TEAM_SCORE_DEF

#define TS_BATTLE_ARENA_PLAYER_SCORE_DEF(_) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6) \
	_(simple)(int16_t, nKillCount) \
	_(simple)(int16_t, nDeathCount) \
	_(simple)(int16_t, nPropActivateCount) \
	_(simple)(int32_t, nTotalGainAP)

CREATE_STRUCT(TS_BATTLE_ARENA_PLAYER_SCORE);
#undef TS_BATTLE_ARENA_PLAYER_SCORE_DEF

#define TS_SC_BATTLE_ARENA_BATTLE_SCORE_DEF(_) \
	_(array)(TS_BATTLE_ARENA_TEAM_SCORE, aTeamScore, 2) \
	_(count)(int8_t, nPlayerScore) \
	_(dynarray)(TS_BATTLE_ARENA_PLAYER_SCORE, nPlayerScore)

// Since EPIC_8_1
#define TS_SC_BATTLE_ARENA_BATTLE_SCORE_ID(X) \
	X(4712, true)

CREATE_PACKET_VER_ID(TS_SC_BATTLE_ARENA_BATTLE_SCORE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BATTLE_ARENA_BATTLE_SCORE_DEF

