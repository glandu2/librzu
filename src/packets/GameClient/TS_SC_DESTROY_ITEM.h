#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_DESTROY_ITEM_DEF(_) \
	_(simple)(ar_handle_t, item_handle)

#define TS_SC_DESTROY_ITEM_ID(X) \
	X(254, version < EPIC_9_6_3) \
	X(1254, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_DESTROY_ITEM, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_DESTROY_ITEM_DEF

