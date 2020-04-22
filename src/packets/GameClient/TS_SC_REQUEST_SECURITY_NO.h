#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_REQUEST_SECURITY_NO_DEF(_) \
	_(simple)(int32_t, mode)

CREATE_PACKET(TS_SC_REQUEST_SECURITY_NO, 9004);
#undef TS_SC_REQUEST_SECURITY_NO_DEF

