#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_TAKEOUT_COMMERCIAL_ITEM_DEF(_) \
	_(simple)(uint32_t, commercial_item_uid) \
	_(simple)(uint16_t, count)

#define TS_CS_TAKEOUT_COMMERCIAL_ITEM_ID(X) \
	X(10005, version < EPIC_9_6_3) \
	X(9005, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_TAKEOUT_COMMERCIAL_ITEM, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_TAKEOUT_COMMERCIAL_ITEM_DEF

