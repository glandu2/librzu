#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BATTLE_ARENA_DISCONNECT_BATTLE_DEF(_) \
	_(simple)(ar_handle_t, handle)

// Since EPIC_8_1
#define TS_SC_BATTLE_ARENA_DISCONNECT_BATTLE_ID(X) \
	X(4714, true)

CREATE_PACKET_VER_ID(TS_SC_BATTLE_ARENA_DISCONNECT_BATTLE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BATTLE_ARENA_DISCONNECT_BATTLE_DEF

