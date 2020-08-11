#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SKILLCARD_INFO_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(ar_handle_t, target_handle)

#define TS_SC_SKILLCARD_INFO_ID(X) \
	X(286, version < EPIC_9_6_3) \
	X(1286, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_SKILLCARD_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_SKILLCARD_INFO_DEF

