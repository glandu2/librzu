#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_INVENTORY.h"

#define TS_CS_BUY_FROM_BOOTH_DEF(_) \
	_(simple)(ar_handle_t, target) \
	_(count)(int16_t, items) \
	_(dynarray)(TS_ITEM_BASE_INFO, items)

#define TS_CS_BUY_FROM_BOOTH_ID(X) \
	X(705, version < EPIC_9_6_3) \
	X(1705, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_BUY_FROM_BOOTH, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_BUY_FROM_BOOTH_DEF

