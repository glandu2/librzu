#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_ITEM_KEEPING_LIST_DEF(_) \
	_(simple)(int32_t, page_num)

#define TS_CS_ITEM_KEEPING_LIST_ID(X) \
	X(1350, version < EPIC_9_6_3) \
	X(2350, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_ITEM_KEEPING_LIST, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_ITEM_KEEPING_LIST_DEF

