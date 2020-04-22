#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_REGION_UPDATE_DEF(_) \
	_(simple)(ar_time_t, update_time) \
	_(simple)(float, x) \
	_(simple)(float, y) \
	_(simple)(float, z) \
	_(simple)(bool, bIsStopMessage)

#define TS_CS_REGION_UPDATE_ID(X) \
	X(7, version < EPIC_9_2) \
	X(67, version >= EPIC_9_2)

CREATE_PACKET_VER_ID(TS_CS_REGION_UPDATE);
#undef TS_CS_REGION_UPDATE_DEF

