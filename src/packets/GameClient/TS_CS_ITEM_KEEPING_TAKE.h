#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_ITEM_KEEPING_TAKE_DEF(_) \
	_(simple)(int32_t, keeping_uid)

#define TS_CS_ITEM_KEEPING_TAKE_ID(X) \
	X(1352, version < EPIC_9_6_3) \
	X(2352, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_ITEM_KEEPING_TAKE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_ITEM_KEEPING_TAKE_DEF

