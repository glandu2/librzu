#ifndef PACKETS_TS_SC_GAME_TIME_H
#define PACKETS_TS_SC_GAME_TIME_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_GAME_TIME_DEF(_) \
	_(simple) (uint32_t, t) \
	_(simple)(def) (uint64_t, game_time) \
	_(simple)(impl)(uint64_t, game_time, version >= EPIC_8_1) \
	_(simple)(impl)(uint32_t, game_time, version < EPIC_8_1)

CREATE_PACKET(TS_SC_GAME_TIME, 1101);

#endif // PACKETS_TS_SC_GAME_TIME_H
