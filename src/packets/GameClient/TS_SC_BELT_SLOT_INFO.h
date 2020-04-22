#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BELT_SLOT_INFO_DEF(_) \
	_(def)(array) (ar_handle_t, handle, 8) \
	  _(impl)(array) (ar_handle_t, handle, 6, version < EPIC_9_5) \
	  _(impl)(array) (ar_handle_t, handle, 8, version >= EPIC_9_5) \

CREATE_PACKET(TS_SC_BELT_SLOT_INFO, 216);
#undef TS_SC_BELT_SLOT_INFO_DEF

