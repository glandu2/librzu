#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_REMOVE_PET_INFO_DEF(_) \
	_(simple)(ar_handle_t, handle)

#define TS_SC_REMOVE_PET_INFO_ID(X) \
	X(352, version < EPIC_9_6_3) \
	X(1352, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_REMOVE_PET_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_REMOVE_PET_INFO_DEF

