#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_COMPETE_START_DEF(_) \
	_(simple)(int8_t, compete_type) \
	_(string)(competitor, 31)

#define TS_SC_COMPETE_START_ID(X) \
	X(4505, true)

CREATE_PACKET_VER_ID(TS_SC_COMPETE_START, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_COMPETE_START_DEF

