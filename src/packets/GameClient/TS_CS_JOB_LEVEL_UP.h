#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_JOB_LEVEL_UP_DEF(_) \
	_(simple)(ar_handle_t, target)

#define TS_CS_JOB_LEVEL_UP_ID(X) \
	X(410, version < EPIC_9_6_3) \
	X(1410, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_JOB_LEVEL_UP, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_JOB_LEVEL_UP_DEF

