#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_AUCTION_BIDDED_LIST_DEF(_) \
	_(simple)(int32_t, page_num)

CREATE_PACKET(TS_CS_AUCTION_BIDDED_LIST, 1304);
#undef TS_CS_AUCTION_BIDDED_LIST_DEF

