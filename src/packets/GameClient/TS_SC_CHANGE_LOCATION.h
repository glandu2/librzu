#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_CHANGE_LOCATION_DEF(_) \
	_(simple) (uint32_t, prev_location_id) \
	_(simple) (uint32_t, cur_location_id)

#define TS_SC_CHANGE_LOCATION_ID(X) \
	X(901, version < EPIC_9_6_3) \
	X(1901, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_CHANGE_LOCATION, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_CHANGE_LOCATION_DEF

