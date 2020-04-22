#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BATTLE_ARENA_RECONNECT_BATTLE_DEF(_) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6) \
	_(simple)(ar_handle_t, newHandle)

// Since EPIC_8_1
CREATE_PACKET(TS_SC_BATTLE_ARENA_RECONNECT_BATTLE, 4715);
#undef TS_SC_BATTLE_ARENA_RECONNECT_BATTLE_DEF

