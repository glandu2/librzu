#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_TRANSMIT_ETHEREAL_DURABILITY_TO_EQUIPMENT_DEF(_) \
	_(simple)(float, rate) \
	_(simple)(int8_t, target, version >= EPIC_8_1, 0)

// target = 0 for player or X for summon (1 to 6)

// Since EPIC_7_2
#define TS_CS_TRANSMIT_ETHEREAL_DURABILITY_TO_EQUIPMENT_ID(X) \
	X(264, version < EPIC_9_6_3) \
	X(1264, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_TRANSMIT_ETHEREAL_DURABILITY_TO_EQUIPMENT, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_TRANSMIT_ETHEREAL_DURABILITY_TO_EQUIPMENT_DEF

