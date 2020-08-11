#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_BATTLE_ARENA_LEAVE_DEF(_)

// Since EPIC_8_1
#define TS_CS_BATTLE_ARENA_LEAVE_ID(X) \
	X(4704, true)

CREATE_PACKET_VER_ID(TS_CS_BATTLE_ARENA_LEAVE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_BATTLE_ARENA_LEAVE_DEF

