#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SKIN_INFO_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int32_t, nSkinColor)

#define TS_SC_SKIN_INFO_ID(X) \
	X(224, version < EPIC_9_6_3) \
	X(1224, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_SKIN_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_SKIN_INFO_DEF

