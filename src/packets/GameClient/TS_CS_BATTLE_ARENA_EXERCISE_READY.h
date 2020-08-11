#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_BATTLE_ARENA_EXERCISE_READY_DEF(_) \
	_(simple)(bool, bReady)

// Since EPIC_8_1
#define TS_CS_BATTLE_ARENA_EXERCISE_READY_ID(X) \
	X(4708, true)

CREATE_PACKET_VER_ID(TS_CS_BATTLE_ARENA_EXERCISE_READY, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_BATTLE_ARENA_EXERCISE_READY_DEF

