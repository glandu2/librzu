#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHARACTER_LIST_DEF(_) \
	_(def)(string)(account, 64) \
	  _(impl)(string)(account, 64, version >= EPIC_9_6) \
	  _(impl)(string)(account, 61, version >= EPIC_5_2 && version < EPIC_9_6) \
	  _(impl)(string)(account, 19, version <  EPIC_5_2) \

#define TS_CS_CHARACTER_LIST_ID(X) \
	X(2407, version >= EPIC_9_6_3) \
	X(2007, version >= EPIC_9_4 && version != EPIC_9_4_AR && version < EPIC_9_6_3) \
	X(2179, version == EPIC_9_4_AR) \
	X(2008, version.flag(EPIC_FLAG_TESTSERVER)) /* 2008 is used on test server */ \
	X(2001, version < EPIC_9_4)

CREATE_PACKET_VER_ID(TS_CS_CHARACTER_LIST, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_CHARACTER_LIST_DEF

