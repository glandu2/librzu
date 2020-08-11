#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_USE_ITEM_RESULT_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(ar_handle_t, target_handle)

#define TS_SC_USE_ITEM_RESULT_ID(X) \
	X(283, version < EPIC_9_6_3) \
	X(1283, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_USE_ITEM_RESULT, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_USE_ITEM_RESULT_DEF

