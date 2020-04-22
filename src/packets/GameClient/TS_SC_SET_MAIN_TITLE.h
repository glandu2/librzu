#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SET_MAIN_TITLE_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int32_t, code)

// Since EPIC_8_1
CREATE_PACKET(TS_SC_SET_MAIN_TITLE, 629);
#undef TS_SC_SET_MAIN_TITLE_DEF

