#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SKILL_CASTING_TIME_DEF(_) \
	_(simple)(ar_handle_t, caster) \
	_(simple)(ar_time_t, tm)

#define TS_SC_SKILL_CASTING_TIME_ID(X) \
	X(405, version < EPIC_9_6_3) \
	X(1405, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_SKILL_CASTING_TIME, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_SKILL_CASTING_TIME_DEF

