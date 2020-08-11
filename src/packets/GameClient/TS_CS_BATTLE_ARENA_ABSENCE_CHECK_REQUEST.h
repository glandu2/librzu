#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_BATTLE_ARENA_ABSENCE_CHECK_REQUEST_DEF(_) \
	_(simple)(uint32_t, hCheckTarget)

// Since EPIC_8_1
#define TS_CS_BATTLE_ARENA_ABSENCE_CHECK_REQUEST_ID(X) \
	X(4717, true)

CREATE_PACKET_VER_ID(TS_CS_BATTLE_ARENA_ABSENCE_CHECK_REQUEST, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_BATTLE_ARENA_ABSENCE_CHECK_REQUEST_DEF

