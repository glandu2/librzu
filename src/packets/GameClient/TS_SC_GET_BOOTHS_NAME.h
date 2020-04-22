#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_BOOTH_NAME_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(string)(name, 49)
CREATE_STRUCT(TS_BOOTH_NAME);
#undef TS_BOOTH_NAME_DEF

#define TS_SC_GET_BOOTHS_NAME_DEF(_) \
	_(count)(int32_t, booths) \
	_(dynarray)(TS_BOOTH_NAME, booths)

CREATE_PACKET(TS_SC_GET_BOOTHS_NAME, 708);
#undef TS_SC_GET_BOOTHS_NAME_DEF

