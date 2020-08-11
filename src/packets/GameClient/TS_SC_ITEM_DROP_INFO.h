#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_ITEM_DROP_INFO_DEF(_) \
	_(simple)(ar_handle_t, monster_handle) \
	_(simple)(ar_handle_t, item_handle)

#define TS_SC_ITEM_DROP_INFO_ID(X) \
	X(282, version < EPIC_9_6_3) \
	X(1282, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_ITEM_DROP_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_ITEM_DROP_INFO_DEF

