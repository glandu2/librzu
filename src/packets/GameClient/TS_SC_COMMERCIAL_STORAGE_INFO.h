#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_COMMERCIAL_STORAGE_INFO_DEF(_) \
	_(simple) (uint16_t, total_item_count) \
	_(simple) (uint16_t, new_item_count)

#define TS_SC_COMMERCIAL_STORAGE_INFO_ID(X) \
	X(10003, version < EPIC_9_6_3) \
	X(9003, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_COMMERCIAL_STORAGE_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_COMMERCIAL_STORAGE_INFO_DEF

