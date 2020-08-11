#pragma once

#include "Packet/PacketDeclaration.h"

enum TS_BATTLE_GRADE : int32_t
{
  BG_INVALID = -1,
  BG_ROOKIE = 0,
  BG_GROW = 1,
  BG_MAJOR = 2,
  MAX_BATTLE_GRADE = 3,
};

#define TS_BATTLE_ARENA_PLAYER_INFO_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int32_t, nJobID) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6)

CREATE_STRUCT(TS_BATTLE_ARENA_PLAYER_INFO);
#undef TS_BATTLE_ARENA_PLAYER_INFO_DEF

#define TS_SC_BATTLE_ARENA_BATTLE_INFO_DEF(_) \
	_(simple)(int32_t, nArenaID) \
	_(simple)(ar_time_t, nForceEnterTime) \
	_(simple)(ar_time_t, nStartTime) \
	_(simple)(TS_BATTLE_GRADE, eGrade) \
	_(simple)(ar_time_t, nEndTime) \
	_(count)(int8_t, team_1) \
	_(count)(int8_t, team_2) \
	_(dynarray)(TS_BATTLE_ARENA_PLAYER_INFO, team_1) \
	_(dynarray)(TS_BATTLE_ARENA_PLAYER_INFO, team_2)

// Since EPIC_8_1
#define TS_SC_BATTLE_ARENA_BATTLE_INFO_ID(X) \
	X(4706, true)

CREATE_PACKET_VER_ID(TS_SC_BATTLE_ARENA_BATTLE_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BATTLE_ARENA_BATTLE_INFO_DEF

