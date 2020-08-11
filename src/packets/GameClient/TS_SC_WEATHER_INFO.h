#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_WEATHER_INFO_DEF(_) \
	_(simple) (uint32_t, region_id) \
	_(simple) (uint16_t, weather_id)

#define TS_SC_WEATHER_INFO_ID(X) \
	X(902, version < EPIC_9_6_3) \
	X(1902, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_WEATHER_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_WEATHER_INFO_DEF

