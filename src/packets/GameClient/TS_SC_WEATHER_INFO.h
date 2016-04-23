#ifndef PACKETS_TS_SC_WEATHER_INFO_H
#define PACKETS_TS_SC_WEATHER_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_WEATHER_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (uint32_t, region_id) \
	simple_ (uint16_t, weather_id)

CREATE_PACKET(TS_SC_WEATHER_INFO, 902);

#endif // PACKETS_TS_SC_WEATHER_INFO_H
