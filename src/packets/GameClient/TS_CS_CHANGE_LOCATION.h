#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHANGE_LOCATION_DEF(_) \
	_(simple) (float, x) \
	_(simple) (float, y)

CREATE_PACKET(TS_CS_CHANGE_LOCATION, 900);
#undef TS_CS_CHANGE_LOCATION_DEF

