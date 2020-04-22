#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_ANTI_HACK_DEF(_) \
	_(simple)(uint16_t, nLength) \
	_(array)(uint8_t, byBuffer, 400)

CREATE_PACKET(TS_CS_ANTI_HACK, 54);
#undef TS_CS_ANTI_HACK_DEF

