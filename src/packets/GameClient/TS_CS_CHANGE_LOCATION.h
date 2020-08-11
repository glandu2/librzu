#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHANGE_LOCATION_DEF(_) \
	_(simple) (float, x) \
	_(simple) (float, y)

#define TS_CS_CHANGE_LOCATION_ID(X) \
	X(900, version < EPIC_9_6_3) \
	X(1900, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_CHANGE_LOCATION, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_CHANGE_LOCATION_DEF

