#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_WEATHER_INFO_DEF(_) \
	_(simple) (uint32_t, region_id) \
	_(simple) (uint16_t, weather_id)

CREATE_PACKET(TS_SC_WEATHER_INFO, 902);
#undef TS_SC_WEATHER_INFO_DEF

