#pragma once

#include "Packet/PacketDeclaration.h"

// Seems unused
#define TS_SC_TARGET_DEF(_) \
	_(simple)(ar_handle_t, target)

#define TS_SC_TARGET_ID(X) \
	X(512, version < EPIC_9_6_3) \
	X(1512, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_TARGET, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_TARGET_DEF

