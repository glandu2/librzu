#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_BATTLE_ARENA_ENTER_WHILE_COUNTDOWN_DEF(_)

// Since EPIC_8_1
#define TS_CS_BATTLE_ARENA_ENTER_WHILE_COUNTDOWN_ID(X) \
	X(4707, true)

CREATE_PACKET_VER_ID(TS_CS_BATTLE_ARENA_ENTER_WHILE_COUNTDOWN, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_BATTLE_ARENA_ENTER_WHILE_COUNTDOWN_DEF

