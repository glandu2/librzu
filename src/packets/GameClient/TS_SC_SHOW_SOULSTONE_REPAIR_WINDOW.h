#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SHOW_SOULSTONE_REPAIR_WINDOW_DEF(_)

#define TS_SC_SHOW_SOULSTONE_REPAIR_WINDOW_ID(X) \
	X(261, version < EPIC_9_6_3) \
	X(1261, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_SHOW_SOULSTONE_REPAIR_WINDOW, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_SHOW_SOULSTONE_REPAIR_WINDOW_DEF

