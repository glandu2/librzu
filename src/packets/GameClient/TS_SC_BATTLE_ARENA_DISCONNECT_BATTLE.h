#ifndef PACKETS_TS_SC_BATTLE_ARENA_DISCONNECT_BATTLE_H
#define PACKETS_TS_SC_BATTLE_ARENA_DISCONNECT_BATTLE_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_BATTLE_ARENA_DISCONNECT_BATTLE_DEF(_) \
	_(simple)(uint32_t, handle)

CREATE_PACKET(TS_SC_BATTLE_ARENA_DISCONNECT_BATTLE, 4714);

#endif // PACKETS_TS_SC_BATTLE_ARENA_DISCONNECT_BATTLE_H
