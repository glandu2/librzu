#pragma once

#include "Packet/PacketDeclaration.h"

enum TS_ARENA_LEAVE_TYPE : int32_t
{
  ALT_UNKNOWN = 0,
  ALT_USER_REQUEST = 1,
  ALT_GAME_OVER = 2,
  ALT_PARTICIPATING_SPECIAL_PARTY = 3,
  ALT_FAILED_TO_JOIN_BATTLE = 4,
  ALT_FAILED_TO_START_BATTLE = 5,
  ALT_DISCONNECT = 6,
  ALT_ABSENT = 7,
  ALT_LEAVE_ARENA = 8,
  ALT_ENTER_INSTANCE_DUNGEON = 9,
  ALT_ENTER_DEATHMATCH = 10,
  ALT_ENTER_HUNTAHOLIC = 11,
  ALT_EXERCISE_PARTY_KICK = 12,
};

#define TS_SC_BATTLE_ARENA_LEAVE_DEF(_) \
	_(simple)(TS_ARENA_LEAVE_TYPE, eLeaveType) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6)

// Since EPIC_8_1
#define TS_SC_BATTLE_ARENA_LEAVE_ID(X) \
	X(4705, true)

CREATE_PACKET_VER_ID(TS_SC_BATTLE_ARENA_LEAVE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BATTLE_ARENA_LEAVE_DEF

