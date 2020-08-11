#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_TAKE_ITEM_DEF(_) \
	_(simple)(ar_handle_t, taker_handle, version >= EPIC_5_2) \
	_(simple)(ar_handle_t, item_handle)

#define TS_CS_TAKE_ITEM_ID(X) \
	X(204, version < EPIC_9_6_3) \
	X(1204, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_TAKE_ITEM, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_TAKE_ITEM_DEF

