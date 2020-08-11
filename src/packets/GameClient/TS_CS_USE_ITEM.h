#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_USE_ITEM_DEF(_) \
	_(simple) (ar_handle_t, item_handle) \
	_(simple) (ar_handle_t, target_handle) \
	_(string) (szParameter, 32, version >= EPIC_4_1)

#define TS_CS_USE_ITEM_ID(X) \
	X(253, version < EPIC_9_6_3) \
	X(1253, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_USE_ITEM, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_USE_ITEM_DEF

