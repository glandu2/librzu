#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_AUCTION_INSTANT_PURCHASE_DEF(_) \
	_(simple)(int32_t, auction_uid)

CREATE_PACKET(TS_CS_AUCTION_INSTANT_PURCHASE, 1308);
#undef TS_CS_AUCTION_INSTANT_PURCHASE_DEF

