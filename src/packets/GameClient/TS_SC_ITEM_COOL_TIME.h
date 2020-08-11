#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_ITEM_COOL_TIME_DEF(_) \
	_(def)(array) (ar_time_t, cool_time, 40) \
	_(impl)(array)(ar_time_t, cool_time, 40, version >= EPIC_6_2) \
	_(impl)(array)(ar_time_t, cool_time, 20, version < EPIC_6_2)

#define TS_SC_ITEM_COOL_TIME_ID(X) \
	X(217, version < EPIC_9_6_3) \
	X(1217, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_ITEM_COOL_TIME, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_ITEM_COOL_TIME_DEF

