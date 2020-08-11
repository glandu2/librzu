#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_AUCTION_SEARCH.h"

#define TS_BIDDED_AUCTION_LIST_DEF(_) \
	_(simple) (TS_AUCTION_INFO, auction_info) \
	_(simple) (uint8_t, status)
CREATE_STRUCT(TS_BIDDED_AUCTION_LIST);
#undef TS_BIDDED_AUCTION_LIST_DEF

#define TS_SC_AUCTION_BIDDED_LIST_DEF(_) \
	_(simple)(int32_t, page_num) \
	_(simple)(int32_t, total_page_count) \
	_(simple)(int32_t, auction_info_count) \
	_(array) (TS_BIDDED_AUCTION_LIST, auction_info, 40)

#define TS_SC_AUCTION_BIDDED_LIST_ID(X) \
	X(1305, version < EPIC_9_6_3) \
	X(2305, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_AUCTION_BIDDED_LIST, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_AUCTION_BIDDED_LIST_DEF

