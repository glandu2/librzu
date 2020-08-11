#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_BATTLE_ARENA_JOIN_QUEUE_DEF(_) \
	_(simple)(int32_t, nArenaID)

// Since EPIC_8_1
#define TS_CS_BATTLE_ARENA_JOIN_QUEUE_ID(X) \
	X(4701, true)

CREATE_PACKET_VER_ID(TS_CS_BATTLE_ARENA_JOIN_QUEUE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_BATTLE_ARENA_JOIN_QUEUE_DEF

