#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_TRANSMIT_ETHEREAL_DURABILITY_TO_EQUIPMENT_DEF(_) \
	_(simple)(float, rate) \
	_(simple)(int8_t, target, version >= EPIC_8_1, 0)

// target = 0 for player or X for summon (1 to 6)

// Since EPIC_7_2
CREATE_PACKET(TS_CS_TRANSMIT_ETHEREAL_DURABILITY_TO_EQUIPMENT, 264);
#undef TS_CS_TRANSMIT_ETHEREAL_DURABILITY_TO_EQUIPMENT_DEF

