#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SKILL_CASTING_TIME_DEF(_) \
	_(simple)(ar_handle_t, caster) \
	_(simple)(ar_time_t, tm)

CREATE_PACKET(TS_SC_SKILL_CASTING_TIME, 405);
#undef TS_SC_SKILL_CASTING_TIME_DEF

