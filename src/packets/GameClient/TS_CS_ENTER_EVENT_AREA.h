#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_ENTER_EVENT_AREA_DEF(_) \
	_(simple)(int32_t, event_area_id) \
	_(simple)(int32_t, area_index)

// Since EPIC_6_3
#define TS_CS_ENTER_EVENT_AREA_ID(X) \
	X(15, version < EPIC_9_6_3) \
	X(1015, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_ENTER_EVENT_AREA, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_ENTER_EVENT_AREA_DEF

