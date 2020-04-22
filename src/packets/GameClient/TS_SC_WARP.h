#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_WARP_DEF(_) \
	_(simple)(float, x) \
	_(simple)(float, y) \
	_(simple)(float, z) \
	_(simple)(int8_t, layer)

CREATE_PACKET(TS_SC_WARP, 12);
#undef TS_SC_WARP_DEF

