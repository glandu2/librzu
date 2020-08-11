#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BOOKMARK_TITLE_DEF(_) \
	_(simple)(int32_t, code) \
	_(simple)(bool, bookmarked)

// Since EPIC_8_1
#define TS_SC_BOOKMARK_TITLE_ID(X) \
	X(633, version < EPIC_9_6_3) \
	X(1633, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_BOOKMARK_TITLE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BOOKMARK_TITLE_DEF

