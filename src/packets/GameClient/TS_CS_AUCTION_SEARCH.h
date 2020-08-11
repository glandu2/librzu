#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_AUCTION_SEARCH_DEF(_) \
	_(simple)(int32_t, category_id) \
	_(simple)(int32_t, sub_category_id) \
	_(string)(keyword, 31) \
	_(simple)(int32_t, page_num) \
	_(simple)(bool, is_equipable, version >= EPIC_7_2)

#define TS_CS_AUCTION_SEARCH_ID(X) \
	X(1300, version < EPIC_9_6_3) \
	X(2300, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_AUCTION_SEARCH, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_AUCTION_SEARCH_DEF

