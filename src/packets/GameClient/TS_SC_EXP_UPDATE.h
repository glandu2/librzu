#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_EXP_UPDATE_DEF(_) \
	_(simple) (ar_handle_t, handle) \
	_(simple) (uint64_t, exp) \
	_(def)(simple) (uint64_t, jp) \
	_(impl)(simple)(uint64_t, jp, version >= EPIC_7_3) \
	_(impl)(simple)(uint32_t, jp, version < EPIC_7_3)

#define TS_SC_EXP_UPDATE_ID(X) \
	X(1003, version < EPIC_9_6_3) \
	X(2003, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_EXP_UPDATE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_EXP_UPDATE_DEF

