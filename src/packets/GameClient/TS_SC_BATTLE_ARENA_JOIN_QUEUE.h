#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_BATTLE_ARENA_BATTLE_INFO.h"

#define TS_SC_BATTLE_ARENA_JOIN_QUEUE_DEF(_) \
	_(simple)(int32_t, nArenaID) \
	_(simple)(TS_BATTLE_GRADE, eGrade)

// Since EPIC_8_1
#define TS_SC_BATTLE_ARENA_JOIN_QUEUE_ID(X) \
	X(4702, true)

CREATE_PACKET_VER_ID(TS_SC_BATTLE_ARENA_JOIN_QUEUE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BATTLE_ARENA_JOIN_QUEUE_DEF

