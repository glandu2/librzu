#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHECK_ILLEGAL_USER_DEF(_) \
	_(simple)(uint32_t, log_code)

CREATE_PACKET(TS_CS_CHECK_ILLEGAL_USER, 57);
#undef TS_CS_CHECK_ILLEGAL_USER_DEF

