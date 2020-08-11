#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_DETECT_RANGE_UPDATE_DEF(_) \
	_(simple) (ar_handle_t, handle) \
	_(simple) (float, detect_range)

// Since EPIC_8_1
#define TS_SC_DETECT_RANGE_UPDATE_ID(X) \
	X(1005, version < EPIC_9_6_3) \
	X(2005, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_DETECT_RANGE_UPDATE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_DETECT_RANGE_UPDATE_DEF

