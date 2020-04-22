#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_RAGE_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int16_t, rage)

CREATE_PACKET(TS_SC_RAGE, 506);
#undef TS_SC_RAGE_DEF

