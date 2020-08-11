#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_UNSUMMON_DEF(_) \
	_(simple)(ar_handle_t, summon_handle)

#define TS_SC_UNSUMMON_ID(X) \
	X(305, version < EPIC_9_6_3) \
	X(1305, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_UNSUMMON, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_UNSUMMON_DEF

