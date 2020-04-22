#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_PUTOFF_CARD_DEF(_) \
	_(simple)(int8_t, position)

CREATE_PACKET(TS_CS_PUTOFF_CARD, 215);
#undef TS_CS_PUTOFF_CARD_DEF

