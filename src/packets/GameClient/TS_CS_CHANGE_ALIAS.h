#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHANGE_ALIAS_DEF(_) \
	_(string)(alias, 19)

// Since EPIC_8_1
#define TS_CS_CHANGE_ALIAS_ID(X) \
	X(31, version < EPIC_9_6_3) \
	X(1031, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_CHANGE_ALIAS, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_CHANGE_ALIAS_DEF

