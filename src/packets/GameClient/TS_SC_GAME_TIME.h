#pragma once

#include "Packet/PacketDeclaration.h"

// checked with e4
#define TS_SC_GAME_TIME_DEF(_) \
	_(simple) (ar_time_t, t) \
	_(def)(simple) (uint64_t, game_time) \
	_(impl)(simple)(uint64_t, game_time, version >= EPIC_4_1_1) \
	_(impl)(simple)(uint32_t, game_time, version < EPIC_4_1_1)

#define TS_SC_GAME_TIME_ID(X) \
	X(1101, version < EPIC_9_6_3) \
	X(2101, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_GAME_TIME, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_GAME_TIME_DEF

