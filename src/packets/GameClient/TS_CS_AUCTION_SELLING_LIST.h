#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_AUCTION_SELLING_LIST_DEF(_) \
	_(simple)(int32_t, page_num)

CREATE_PACKET(TS_CS_AUCTION_SELLING_LIST, 1302);
#undef TS_CS_AUCTION_SELLING_LIST_DEF

