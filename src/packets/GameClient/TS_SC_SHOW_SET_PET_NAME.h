#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SHOW_SET_PET_NAME_DEF(_) \
	_(simple)(ar_handle_t, handle)

#define TS_SC_SHOW_SET_PET_NAME_ID(X) \
	X(353, version < EPIC_9_6_3) \
	X(1353, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_SHOW_SET_PET_NAME, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_SHOW_SET_PET_NAME_DEF

