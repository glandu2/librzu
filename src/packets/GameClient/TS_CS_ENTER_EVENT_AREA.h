#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_ENTER_EVENT_AREA_DEF(_) \
	_(simple)(int32_t, event_area_id) \
	_(simple)(int32_t, area_index)

// Since EPIC_6_3
CREATE_PACKET(TS_CS_ENTER_EVENT_AREA, 15);
#undef TS_CS_ENTER_EVENT_AREA_DEF

