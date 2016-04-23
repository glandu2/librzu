#ifndef PACKETS_TS_SC_CHANGE_LOCATION_H
#define PACKETS_TS_SC_CHANGE_LOCATION_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_CHANGE_LOCATION_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (uint32_t, prev_location_id) \
	simple_ (uint32_t, cur_location_id)

CREATE_PACKET(TS_SC_CHANGE_LOCATION, 901);

#endif // PACKETS_TS_SC_CHANGE_LOCATION_H
