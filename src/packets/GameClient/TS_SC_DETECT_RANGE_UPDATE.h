#ifndef PACKETS_TS_SC_DETECT_RANGE_UPDATE_H
#define PACKETS_TS_SC_DETECT_RANGE_UPDATE_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_DETECT_RANGE_UPDATE_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (uint32_t, handle) \
	simple_ (float, detect_range)

CREATE_PACKET(TS_SC_DETECT_RANGE_UPDATE, 1005);

#endif // PACKETS_TS_SC_DETECT_RANGE_UPDATE_H
