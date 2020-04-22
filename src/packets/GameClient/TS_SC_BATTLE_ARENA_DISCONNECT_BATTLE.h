#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BATTLE_ARENA_DISCONNECT_BATTLE_DEF(_) \
	_(simple)(ar_handle_t, handle)

// Since EPIC_8_1
CREATE_PACKET(TS_SC_BATTLE_ARENA_DISCONNECT_BATTLE, 4714);
#undef TS_SC_BATTLE_ARENA_DISCONNECT_BATTLE_DEF

