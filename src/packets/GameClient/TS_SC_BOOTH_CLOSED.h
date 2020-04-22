#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BOOTH_CLOSED_DEF(_) \
	_(simple)(ar_handle_t, target)

CREATE_PACKET(TS_SC_BOOTH_CLOSED, 709);
#undef TS_SC_BOOTH_CLOSED_DEF

