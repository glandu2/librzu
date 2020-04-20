#ifndef PACKETS_TS_SC_BATTLE_ARENA_JOIN_BATTLE_H
#define PACKETS_TS_SC_BATTLE_ARENA_JOIN_BATTLE_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_BATTLE_ARENA_JOIN_BATTLE_DEF(_) \
	_(simple)(int32_t, nTeamNo) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int32_t, nJobID) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6)

// Since EPIC_8_1
CREATE_PACKET(TS_SC_BATTLE_ARENA_JOIN_BATTLE, 4713);
#undef TS_SC_BATTLE_ARENA_JOIN_BATTLE_DEF

#endif // PACKETS_TS_SC_BATTLE_ARENA_JOIN_BATTLE_H
