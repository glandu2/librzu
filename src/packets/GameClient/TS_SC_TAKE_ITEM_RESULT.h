#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_TAKE_ITEM_RESULT_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(ar_handle_t, item_taker)

#define TS_SC_TAKE_ITEM_RESULT_ID(X) \
	X(210, version < EPIC_9_6_3) \
	X(1210, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_TAKE_ITEM_RESULT, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_TAKE_ITEM_RESULT_DEF

