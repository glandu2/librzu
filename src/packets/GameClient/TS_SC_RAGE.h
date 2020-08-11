#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_RAGE_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int16_t, rage)

#define TS_SC_RAGE_ID(X) \
	X(506, version < EPIC_9_6_3) \
	X(1506, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_RAGE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_RAGE_DEF

