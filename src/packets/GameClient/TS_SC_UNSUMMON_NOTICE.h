#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_UNSUMMON_NOTICE_DEF(_) \
	_(simple)(ar_handle_t, summon_handle) \
	_(simple)(ar_time_t, unsummon_duration)

#define TS_SC_UNSUMMON_NOTICE_ID(X) \
	X(306, version < EPIC_9_6_3) \
	X(1306, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_UNSUMMON_NOTICE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_UNSUMMON_NOTICE_DEF

