#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_COMPETE_ANSWER_DEF(_) \
	_(simple)(int8_t, compete_type) \
	_(simple)(int8_t, answer_type) \
	_(string)(requestee, 31)

CREATE_PACKET(TS_SC_COMPETE_ANSWER, 4503);
#undef TS_SC_COMPETE_ANSWER_DEF

