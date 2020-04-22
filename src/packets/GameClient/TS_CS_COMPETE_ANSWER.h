#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_COMPETE_ANSWER_DEF(_) \
	_(simple)(int8_t, compete_type) \
	_(simple)(int8_t, answer_type)

CREATE_PACKET(TS_CS_COMPETE_ANSWER, 4502);
#undef TS_CS_COMPETE_ANSWER_DEF

