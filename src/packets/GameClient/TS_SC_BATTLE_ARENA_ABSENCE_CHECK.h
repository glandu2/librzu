#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BATTLE_ARENA_ABSENCE_CHECK_DEF(_) \
	_(simple)(ar_time_t, nLimitTime)

// Since EPIC_8_1
#define TS_SC_BATTLE_ARENA_ABSENCE_CHECK_ID(X) \
	X(4718, true)

CREATE_PACKET_VER_ID(TS_SC_BATTLE_ARENA_ABSENCE_CHECK, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BATTLE_ARENA_ABSENCE_CHECK_DEF

