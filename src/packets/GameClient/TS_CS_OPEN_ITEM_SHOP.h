#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_OPEN_ITEM_SHOP_DEF(_)

#define TS_CS_OPEN_ITEM_SHOP_ID(X) \
	X(10000, version < EPIC_9_6_3) \
	X(9000, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_OPEN_ITEM_SHOP, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_OPEN_ITEM_SHOP_DEF

