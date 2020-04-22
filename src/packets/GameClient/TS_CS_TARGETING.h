#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_TARGETING_DEF(_) \
	_(simple)(ar_handle_t, target)

CREATE_PACKET(TS_CS_TARGETING, 511);
#undef TS_CS_TARGETING_DEF

