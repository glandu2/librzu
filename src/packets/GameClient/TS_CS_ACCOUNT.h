#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_ACCOUNT_DEF(_) \
	_(string)(account, 19) \
	_(array)(uint8_t, password, 32)

#define TS_CS_ACCOUNT_ID(X) \
	X(2000, version < EPIC_9_6_3) \
	X(2400, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_ACCOUNT, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_ACCOUNT_DEF

