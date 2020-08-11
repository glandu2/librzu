#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_ACCOUNT_WITH_AUTH_DEF(_) \
	_(def)(string)(account, 61) \
	_(impl)(string)(account, 61, version >= EPIC_5_2) \
	_(impl)(string)(account, 19, version <  EPIC_5_2) \
	_(simple)(uint64_t, one_time_key)

#define TS_CS_ACCOUNT_WITH_AUTH_ID(X) \
	X(2005, version < EPIC_9_6_3) \
	X(2405, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_ACCOUNT_WITH_AUTH, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_ACCOUNT_WITH_AUTH_DEF

