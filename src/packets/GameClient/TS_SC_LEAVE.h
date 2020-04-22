#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_LEAVE_DEF(_) \
	_(simple)(ar_handle_t, handle)

CREATE_PACKET(TS_SC_LEAVE, 9);
#undef TS_SC_LEAVE_DEF

