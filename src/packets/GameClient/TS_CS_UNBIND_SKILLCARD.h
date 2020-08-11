#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_UNBIND_SKILLCARD_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(ar_handle_t, target_handle)

#define TS_CS_UNBIND_SKILLCARD_ID(X) \
	X(285, version < EPIC_9_6_3) \
	X(1285, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_UNBIND_SKILLCARD, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_UNBIND_SKILLCARD_DEF

