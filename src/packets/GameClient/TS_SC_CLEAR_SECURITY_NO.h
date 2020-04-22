#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_CLEAR_SECURITY_NO_DEF(_) \
	_(simple)(bool, clearable) \

CREATE_PACKET(TS_SC_CLEAR_SECURITY_NO, 9011);
#undef TS_SC_CLEAR_SECURITY_NO_DEF

