#ifndef PACKETS_TS_SC_GAME_TIME_H
#define PACKETS_TS_SC_GAME_TIME_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_GAME_TIME_DEF(_) \
	_(simple) (uint32_t, t) \
	_(def)(simple) (uint64_t, game_time) \
	_(impl)(simple)(uint64_t, game_time, version >= EPIC_7_4) \
	_(impl)(simple)(uint32_t, game_time, version < EPIC_7_4)

CREATE_PACKET(TS_SC_GAME_TIME, 1101);

#endif // PACKETS_TS_SC_GAME_TIME_H
