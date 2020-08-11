#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BATTLE_ARENA_RECONNECT_BATTLE_DEF(_) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6) \
	_(simple)(ar_handle_t, newHandle)

// Since EPIC_8_1
#define TS_SC_BATTLE_ARENA_RECONNECT_BATTLE_ID(X) \
	X(4715, true)

CREATE_PACKET_VER_ID(TS_SC_BATTLE_ARENA_RECONNECT_BATTLE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BATTLE_ARENA_RECONNECT_BATTLE_DEF

