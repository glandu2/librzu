#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BELT_SLOT_INFO_DEF(_) \
	_(def)(array) (ar_handle_t, handle, 8) \
	  _(impl)(array) (ar_handle_t, handle, 6, version < EPIC_9_5) \
	  _(impl)(array) (ar_handle_t, handle, 8, version >= EPIC_9_5) \

#define TS_SC_BELT_SLOT_INFO_ID(X) \
	X(216, version < EPIC_9_6_3) \
	X(1216, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_BELT_SLOT_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BELT_SLOT_INFO_DEF

