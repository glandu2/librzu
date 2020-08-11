#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_AUCTION_CANCEL_DEF(_) \
	_(simple)(uint32_t, auction_uid)

#define TS_CS_AUCTION_CANCEL_ID(X) \
	X(1310, version < EPIC_9_6_3) \
	X(2310, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_AUCTION_CANCEL, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_AUCTION_CANCEL_DEF

