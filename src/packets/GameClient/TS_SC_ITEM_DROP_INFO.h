#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_ITEM_DROP_INFO_DEF(_) \
	_(simple)(ar_handle_t, monster_handle) \
	_(simple)(ar_handle_t, item_handle)

CREATE_PACKET(TS_SC_ITEM_DROP_INFO, 282);
#undef TS_SC_ITEM_DROP_INFO_DEF

