#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_COMPETE_ANSWER_DEF(_) \
	_(simple)(int8_t, compete_type) \
	_(simple)(int8_t, answer_type) \
	_(string)(requestee, 31)

#define TS_SC_COMPETE_ANSWER_ID(X) \
	X(4503, true)

CREATE_PACKET_VER_ID(TS_SC_COMPETE_ANSWER, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_COMPETE_ANSWER_DEF

