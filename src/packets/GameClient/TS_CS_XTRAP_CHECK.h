#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_XTRAP_CHECK_DEF(_) \
	_(array)(uint8_t, pCheckBuffer, 128)

CREATE_PACKET(TS_CS_XTRAP_CHECK, 59);
#undef TS_CS_XTRAP_CHECK_DEF

