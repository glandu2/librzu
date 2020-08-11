#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_DELETE_CHARACTER_DEF(_) \
	_(def)(string)(name, 40) \
	_(impl)(string)(name, 40, version >= EPIC_9_6) \
	_(impl)(string)(name, 38, version >= EPIC_8_1 && version < EPIC_9_6) \
	_(impl)(string)(name, 19, version <  EPIC_8_1)

#define TS_CS_DELETE_CHARACTER_ID(X) \
	X(2003, version < EPIC_9_6_3) \
	X(2403, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_DELETE_CHARACTER, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_DELETE_CHARACTER_DEF

