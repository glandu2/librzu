#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_LEVEL_UPDATE_DEF(_) \
	_(simple) (ar_handle_t, handle) \
	_(simple) (int32_t, level) \
	_(simple) (int32_t, job_level)

#define TS_SC_LEVEL_UPDATE_ID(X) \
	X(1002, version < EPIC_9_6_3) \
	X(2002, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_LEVEL_UPDATE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_LEVEL_UPDATE_DEF

