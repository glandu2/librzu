#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BATTLE_ARENA_BATTLE_STATUS_DEF(_) \
	_(simple)(int32_t, nStatus)

// Since EPIC_8_1
#define TS_SC_BATTLE_ARENA_BATTLE_STATUS_ID(X) \
	X(4711, true)

CREATE_PACKET_VER_ID(TS_SC_BATTLE_ARENA_BATTLE_STATUS, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BATTLE_ARENA_BATTLE_STATUS_DEF

