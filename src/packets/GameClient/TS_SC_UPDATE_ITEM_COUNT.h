#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_UPDATE_ITEM_COUNT_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(def)(simple)(int64_t, count) \
	_(impl)(simple)(int64_t, count, version >= EPIC_4_1) \
	_(impl)(simple)(int32_t, count, version < EPIC_4_1)

#define TS_SC_UPDATE_ITEM_COUNT_ID(X) \
	X(255, version < EPIC_9_6_3) \
	X(1255, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_UPDATE_ITEM_COUNT, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_UPDATE_ITEM_COUNT_DEF

