#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_ARRANGE_ITEM_DEF(_) \
	_(simple)(bool, bIsStorage)

#define TS_CS_ARRANGE_ITEM_ID(X) \
	X(219, version < EPIC_9_6_3) \
	X(1219, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_ARRANGE_ITEM, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_ARRANGE_ITEM_DEF

