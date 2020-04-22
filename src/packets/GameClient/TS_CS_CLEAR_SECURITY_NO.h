#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_CLEAR_SECURITY_NO_DEF(_) \
	_(array)(uint8_t, password, 19) \
	_(array)(uint8_t, security_no, 19)

CREATE_PACKET(TS_CS_CLEAR_SECURITY_NO, 9012);
#undef TS_CS_CLEAR_SECURITY_NO_DEF

