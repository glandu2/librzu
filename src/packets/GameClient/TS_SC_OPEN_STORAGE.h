#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_OPEN_STORAGE_DEF(_) \
	_(simple)(int32_t, maxStorageItemCount, version >= EPIC_7_4, 10000)

#define TS_SC_OPEN_STORAGE_ID(X) \
	X(211, version < EPIC_9_6_3) \
	X(1211, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_OPEN_STORAGE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_OPEN_STORAGE_DEF

