#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_TAKE_ITEM_DEF(_) \
	_(simple)(ar_handle_t, taker_handle, version >= EPIC_5_2) \
	_(simple)(ar_handle_t, item_handle)

CREATE_PACKET(TS_CS_TAKE_ITEM, 204);
#undef TS_CS_TAKE_ITEM_DEF

