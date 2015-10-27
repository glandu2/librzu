#ifndef PACKETS_TS_SC_WARP_H
#define PACKETS_TS_SC_WARP_H

#include "PacketDeclaration.h"

#define TS_SC_WARP_DEF(simple_, array_, dynarray_, count_) \
	simple_(float, x) \
	simple_(float, y) \
	simple_(float, z) \
	simple_(char, layer)

CREATE_PACKET(TS_SC_WARP, 12);

#endif // PACKETS_TS_SC_WARP_H