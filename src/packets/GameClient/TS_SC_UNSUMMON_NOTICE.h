#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_UNSUMMON_NOTICE_DEF(_) \
	_(simple)(ar_handle_t, summon_handle) \
	_(simple)(ar_time_t, unsummon_duration)

CREATE_PACKET(TS_SC_UNSUMMON_NOTICE, 306);
#undef TS_SC_UNSUMMON_NOTICE_DEF

