#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHANGE_SUMMON_NAME_DEF(_) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6)

#define TS_CS_CHANGE_SUMMON_NAME_ID(X) \
	X(323, version < EPIC_9_6_3) \
	X(1323, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_CHANGE_SUMMON_NAME, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_CHANGE_SUMMON_NAME_DEF

