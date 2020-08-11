#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_REMAIN_TITLE_TIME_DEF(_) \
	_(simple)(ar_time_t, remain_title_time)

// Since EPIC_8_1
#define TS_SC_REMAIN_TITLE_TIME_ID(X) \
	X(627, version < EPIC_9_6_3) \
	X(1627, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_REMAIN_TITLE_TIME, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_REMAIN_TITLE_TIME_DEF

