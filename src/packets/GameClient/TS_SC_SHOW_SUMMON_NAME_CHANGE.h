#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SHOW_SUMMON_NAME_CHANGE_DEF(_) \
	_(simple)(ar_handle_t, handle)

CREATE_PACKET(TS_SC_SHOW_SUMMON_NAME_CHANGE, 322);
#undef TS_SC_SHOW_SUMMON_NAME_CHANGE_DEF

