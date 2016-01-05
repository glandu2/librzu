#ifndef PACKETS_TS_CS_REGION_UPDATE_H
#define PACKETS_TS_CS_REGION_UPDATE_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_REGION_UPDATE_DEF(simple_, array_, dynarray_, count_) \
	simple_(uint32_t, update_time) \
	simple_(float, x) \
	simple_(float, y) \
	simple_(float, z) \
	simple_(bool, bIsStopMessage)

CREATE_PACKET(TS_CS_REGION_UPDATE, 7);

#endif // PACKETS_TS_CS_REGION_UPDATE_H
