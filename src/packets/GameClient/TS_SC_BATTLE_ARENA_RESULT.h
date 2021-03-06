#pragma once

#include "Packet/PacketDeclaration.h"

enum TS_ARENA_END_TYPE : int32_t
{
  AET_UNKNOWN = 0,
  AET_ONE_TEAM_NO_MEMBER = 1,
  AET_MAX_SCORE_REACHED = 2,
  AET_TIME_OVER = 3,
};

enum TS_ARENA_REWARD_TYPE : int32_t
{
  ART_UNKNOWN = 0,
  ART_FULL_REWARD = 1,
  ART_HALF_REWARD_BY_FEW_MEMBER = 2,
  ART_NO_REWARD_BY_NO_MEMBER = 3,
  ART_NO_REWARD_BY_MIN_TIME = 4,
  ART_NO_REWARD_BY_EXERCISE = 5,
};

#define TS_ARENA_MVP_DEF(_) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6)
CREATE_STRUCT(TS_ARENA_MVP);
#undef TS_ARENA_MVP_DEF

#define TS_SC_BATTLE_ARENA_RESULT_DEF(_) \
	_(simple)(TS_ARENA_END_TYPE, eEndType) \
	_(simple)(TS_ARENA_REWARD_TYPE, eRewardType) \
	_(simple)(int32_t, nWinnerTeamNo) \
	_(count)(int8_t, MVPs) \
	_(dynarray)(TS_ARENA_MVP, MVPs)

// Since EPIC_8_1
#define TS_SC_BATTLE_ARENA_RESULT_ID(X) \
	X(4716, true)

CREATE_PACKET_VER_ID(TS_SC_BATTLE_ARENA_RESULT, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BATTLE_ARENA_RESULT_DEF

