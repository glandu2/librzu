#ifndef PACKETS_TS_SC_REGION_ACK_H
#define PACKETS_TS_SC_REGION_ACK_H

#include "PacketDeclaration.h"

#define TS_SC_REGION_ACK_DEF(simple_, array_, dynarray_, count_) \
	simple_(int32_t, rx) \
	simple_(int32_t, rx)

CREATE_PACKET(TS_SC_REGION_ACK, 11);

#endif // PACKETS_TS_SC_REGION_ACK_H