#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SET_SUB_TITLE_DEF(_) \
	_(simple)(int32_t, index) \
	_(simple)(int32_t, code)

// Since EPIC_8_1
CREATE_PACKET(TS_SC_SET_SUB_TITLE, 631);
#undef TS_SC_SET_SUB_TITLE_DEF

