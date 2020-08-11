#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_BUY_ITEM_DEF(_) \
	_(simple)(int32_t, item_code) \
	_(def)(simple) (uint16_t, buy_count) \
	_(impl)(simple)(uint16_t, buy_count, version >= EPIC_4_1) \
	_(impl)(simple)(uint8_t, buy_count, version < EPIC_4_1)

#define TS_CS_BUY_ITEM_ID(X) \
	X(251, version < EPIC_9_6_3) \
	X(1251, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_BUY_ITEM, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_BUY_ITEM_DEF

