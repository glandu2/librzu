#pragma once

#include "Packet/PacketDeclaration.h"

// Sent by the client when receiving a packet about an unknown handle.
// This trigger a TS_SC_ENTER from the GS
#define TS_CS_QUERY_DEF(_) \
	_(simple)(ar_handle_t, handle)

#define TS_CS_QUERY_ID(X) \
	X(13, version < EPIC_9_6_3) \
	X(1013, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_QUERY, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_QUERY_DEF

