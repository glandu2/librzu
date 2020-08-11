#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_OPEN_PAID_STORAGE_DEF(_)

#define TS_SC_OPEN_PAID_STORAGE_ID(X) \
	X(10002, version < EPIC_9_6_3) \
	X(9002, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_OPEN_PAID_STORAGE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_OPEN_PAID_STORAGE_DEF

