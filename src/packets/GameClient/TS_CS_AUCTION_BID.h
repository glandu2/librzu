#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_AUCTION_BID_DEF(_) \
	_(simple)(int32_t, auction_uid) \
	_(simple)(int64_t, price)

#define TS_CS_AUCTION_BID_ID(X) \
	X(1306, version < EPIC_9_6_3) \
	X(2306, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_AUCTION_BID, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_AUCTION_BID_DEF

