#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_TAMING_INFO_DEF(_) \
	_(simple)(int8_t, mode) \
	_(simple)(ar_handle_t, tamer_handle) \
	_(simple)(ar_handle_t, target_handle)

#define TS_SC_TAMING_INFO_ID(X) \
	X(310, version < EPIC_9_6_3) \
	X(1310, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_TAMING_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_TAMING_INFO_DEF

