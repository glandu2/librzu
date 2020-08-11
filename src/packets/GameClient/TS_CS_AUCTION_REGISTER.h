#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_AUCTION_REGISTER_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(int32_t, item_count) \
	_(simple)(int64_t, start_price) \
	_(simple)(int64_t, instant_purchase_price) \
	_(simple)(int8_t, duration_type)

#define TS_CS_AUCTION_REGISTER_ID(X) \
	X(1309, version < EPIC_9_6_3) \
	X(2309, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_AUCTION_REGISTER, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_AUCTION_REGISTER_DEF

