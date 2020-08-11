#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_BATTLE_ARENA_EXERCISE_START_DEF(_)

// Since EPIC_8_1
#define TS_CS_BATTLE_ARENA_EXERCISE_START_ID(X) \
	X(4710, true)

CREATE_PACKET_VER_ID(TS_CS_BATTLE_ARENA_EXERCISE_START, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_BATTLE_ARENA_EXERCISE_START_DEF

