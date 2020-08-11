#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_DROP_RESULT_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(bool, isAccepted)

#define TS_SC_DROP_RESULT_ID(X) \
	X(205, version < EPIC_9_6_3) \
	X(1205, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_DROP_RESULT, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_DROP_RESULT_DEF

