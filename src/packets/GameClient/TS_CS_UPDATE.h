#ifndef PACKETS_TS_CS_UPDATE_H
#define PACKETS_TS_CS_UPDATE_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_UPDATE_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, handle) \
	simple_ (uint32_t, time, version >= EPIC_9_2) \
	simple_ (uint32_t, epoch_time, version >= EPIC_9_2)

CREATE_PACKET(TS_CS_UPDATE, 503);

#endif // PACKETS_TS_CS_UPDATE_H
