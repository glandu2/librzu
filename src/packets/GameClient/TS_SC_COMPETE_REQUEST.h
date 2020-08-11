#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_COMPETE_REQUEST_DEF(_) \
	_(simple)(int8_t, compete_type) \
	_(string)(requester, 31)

#define TS_SC_COMPETE_REQUEST_ID(X) \
	X(4501, true)

CREATE_PACKET_VER_ID(TS_SC_COMPETE_REQUEST, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_COMPETE_REQUEST_DEF

