#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SP_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int16_t, sp) \
	_(simple)(int16_t, max_sp)

#define TS_SC_SP_ID(X) \
	X(514, version < EPIC_9_6_3) \
	X(1514, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_SP, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_SP_DEF

