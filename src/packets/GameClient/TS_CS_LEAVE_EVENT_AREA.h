#ifndef PACKETS_TS_CS_LEAVE_EVENT_AREA_H
#define PACKETS_TS_CS_LEAVE_EVENT_AREA_H

#include "PacketDeclaration.h"

#define TS_CS_LEAVE_EVENT_AREA_DEF(simple_, array_, dynarray_, count_) \
	simple_(int32_t, event_area_id) \
	simple_(int32_t, area_index)

CREATE_PACKET(TS_CS_LEAVE_EVENT_AREA, 16);

#endif // PACKETS_TS_CS_LEAVE_EVENT_AREA_H