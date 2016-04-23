#ifndef PACKETS_TS_SC_REGION_ACK_H
#define PACKETS_TS_SC_REGION_ACK_H

#include "PacketDeclaration.h"

#define TS_SC_REGION_ACK_DEF(_(simple), _(array), _(dynarray), _(count)) \
	_(simple)(int32_t, rx) \
	_(simple)(int32_t, rx)

CREATE_PACKET(TS_SC_REGION_ACK, 11);

#endif // PACKETS_TS_SC_REGION_ACK_H