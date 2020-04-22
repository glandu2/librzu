#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_CHANGE_LOCATION_DEF(_) \
	_(simple) (uint32_t, prev_location_id) \
	_(simple) (uint32_t, cur_location_id)

CREATE_PACKET(TS_SC_CHANGE_LOCATION, 901);
#undef TS_SC_CHANGE_LOCATION_DEF

