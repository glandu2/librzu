#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BATTLE_ARENA_UPDATE_WAIT_USER_COUNT_DEF(_) \
	_(simple)(int32_t, nWaitUserCount)

// Since EPIC_8_1
#define TS_SC_BATTLE_ARENA_UPDATE_WAIT_USER_COUNT_ID(X) \
	X(4703, true)

CREATE_PACKET_VER_ID(TS_SC_BATTLE_ARENA_UPDATE_WAIT_USER_COUNT, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BATTLE_ARENA_UPDATE_WAIT_USER_COUNT_DEF

