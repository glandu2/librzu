#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_DELETE_CHARACTER_DEF(_) \
	_(def)(string)(name, 40) \
	_(impl)(string)(name, 40, version >= EPIC_9_6) \
	_(impl)(string)(name, 38, version >= EPIC_8_1 && version < EPIC_9_6) \
	_(impl)(string)(name, 19, version <  EPIC_8_1)

CREATE_PACKET(TS_CS_DELETE_CHARACTER, 2003);
#undef TS_CS_DELETE_CHARACTER_DEF

