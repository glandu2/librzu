#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_SET_PET_NAME_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6)

CREATE_PACKET(TS_CS_SET_PET_NAME, 354);
#undef TS_CS_SET_PET_NAME_DEF

