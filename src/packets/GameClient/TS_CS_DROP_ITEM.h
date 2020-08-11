#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_DROP_ITEM_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(def)(simple) (int32_t, count) \
	_(impl)(simple)(int32_t, count, version >= EPIC_4_1) \
	_(impl)(simple)(uint16_t, count, version < EPIC_4_1)

#define TS_CS_DROP_ITEM_ID(X) \
	X(203, version < EPIC_9_6_3) \
	X(1203, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_DROP_ITEM, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_DROP_ITEM_DEF

