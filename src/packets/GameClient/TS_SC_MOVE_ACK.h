#ifndef PACKETS_TS_SC_MOVE_ACK_H
#define PACKETS_TS_SC_MOVE_ACK_H

#include "PacketDeclaration.h"

#define TS_SC_MOVE_ACK_DEF(simple_, array_, dynarray_, count_) \
	simple_(uint32_t, time) \
	simple_(char, speed)

CREATE_PACKET(TS_SC_MOVE_ACK, 6);

#endif // PACKETS_TS_SC_MOVE_ACK_H