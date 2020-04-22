#pragma once

#include "Packet/PacketDeclaration.h"

// Seems unused
#define TS_SC_TARGET_DEF(_) \
	_(simple)(ar_handle_t, target)

CREATE_PACKET(TS_SC_TARGET, 512);
#undef TS_SC_TARGET_DEF

