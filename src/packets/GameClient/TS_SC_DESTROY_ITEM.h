#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_DESTROY_ITEM_DEF(_) \
	_(simple)(ar_handle_t, item_handle)

CREATE_PACKET(TS_SC_DESTROY_ITEM, 254);
#undef TS_SC_DESTROY_ITEM_DEF

