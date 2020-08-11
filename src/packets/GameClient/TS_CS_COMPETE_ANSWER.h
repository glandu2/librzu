#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_COMPETE_ANSWER_DEF(_) \
	_(simple)(int8_t, compete_type) \
	_(simple)(int8_t, answer_type)

#define TS_CS_COMPETE_ANSWER_ID(X) \
	X(4502, true)

CREATE_PACKET_VER_ID(TS_CS_COMPETE_ANSWER, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_COMPETE_ANSWER_DEF

