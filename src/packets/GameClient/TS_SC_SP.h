#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SP_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int16_t, sp) \
	_(simple)(int16_t, max_sp)

CREATE_PACKET(TS_SC_SP, 514);
#undef TS_SC_SP_DEF

