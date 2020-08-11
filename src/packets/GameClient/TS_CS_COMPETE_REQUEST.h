#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_COMPETE_REQUEST_DEF(_) \
	_(simple)(int8_t, compete_type) \
	_(string)(requestee, 31)

#define TS_CS_COMPETE_REQUEST_ID(X) \
	X(4500, true)

CREATE_PACKET_VER_ID(TS_CS_COMPETE_REQUEST, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_COMPETE_REQUEST_DEF

