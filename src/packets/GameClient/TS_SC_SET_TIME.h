#ifndef PACKETS_TS_SC_SET_TIME_H
#define PACKETS_TS_SC_SET_TIME_H

#include "PacketDeclaration.h"

#define TS_SC_SET_TIME_DEF(simple_, array_, dynarray_, count_) \
	simple_(int32_t, gap)

CREATE_PACKET(TS_SC_SET_TIME, 10);

#endif // PACKETS_TS_SC_SET_TIME_H