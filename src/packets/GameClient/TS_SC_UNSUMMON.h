#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_UNSUMMON_DEF(_) \
	_(simple)(ar_handle_t, summon_handle)

CREATE_PACKET(TS_SC_UNSUMMON, 305);
#undef TS_SC_UNSUMMON_DEF

