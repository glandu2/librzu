#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BOOTH_CLOSED_DEF(_) \
	_(simple)(ar_handle_t, target)

#define TS_SC_BOOTH_CLOSED_ID(X) \
	X(709, version < EPIC_9_6_3) \
	X(1709, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_BOOTH_CLOSED, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BOOTH_CLOSED_DEF

