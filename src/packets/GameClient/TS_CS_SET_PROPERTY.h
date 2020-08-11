#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_SET_PROPERTY_DEF(_) \
	_(string)(name, 16) \
	_(endstring)(string_value, true)

#define TS_CS_SET_PROPERTY_ID(X) \
	X(508, version < EPIC_9_6_3) \
	X(1508, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_SET_PROPERTY, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_SET_PROPERTY_DEF

