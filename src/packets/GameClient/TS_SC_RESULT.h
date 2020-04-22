#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_RESULT_DEF(_) \
	_(simple)(uint16_t, request_msg_id) \
	_(simple)(uint16_t, result) \
	_(simple)(int32_t, value)

CREATE_PACKET(TS_SC_RESULT, 0);
#undef TS_SC_RESULT_DEF

