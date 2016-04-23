#ifndef PACKETS_TS_SC_GAME_TIME_H
#define PACKETS_TS_SC_GAME_TIME_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_GAME_TIME_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (uint32_t, t) \
	simple_(def) (uint64_t, game_time) \
	simple_(impl)(uint64_t, game_time, version >= EPIC_8_1) \
	simple_(impl)(uint32_t, game_time, version < EPIC_8_1)

CREATE_PACKET(TS_SC_GAME_TIME, 1101);

#endif // PACKETS_TS_SC_GAME_TIME_H
