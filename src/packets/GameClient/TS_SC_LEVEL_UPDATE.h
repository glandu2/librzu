#ifndef PACKETS_TS_SC_LEVEL_UPDATE_H
#define PACKETS_TS_SC_LEVEL_UPDATE_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_LEVEL_UPDATE_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, handle) \
	simple_ (int32_t, level) \
	simple_ (int32_t, job_level)

CREATE_PACKET(TS_SC_LEVEL_UPDATE, 1002);

#endif // PACKETS_TS_SC_LEVEL_UPDATE_H
