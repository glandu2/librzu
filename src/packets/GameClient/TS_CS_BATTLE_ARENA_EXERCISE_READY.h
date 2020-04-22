#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_BATTLE_ARENA_EXERCISE_READY_DEF(_) \
	_(simple)(bool, bReady)

// Since EPIC_8_1
CREATE_PACKET(TS_CS_BATTLE_ARENA_EXERCISE_READY, 4708);
#undef TS_CS_BATTLE_ARENA_EXERCISE_READY_DEF

