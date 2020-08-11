#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_GET_WEATHER_INFO_DEF(_) \
	_(simple)(uint32_t, region_id)

#define TS_CS_GET_WEATHER_INFO_ID(X) \
	X(903, version < EPIC_9_6_3) \
	X(1903, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_GET_WEATHER_INFO, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_GET_WEATHER_INFO_DEF

