#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_INVENTORY.h"

#define TS_CS_BUY_FROM_BOOTH_DEF(_) \
	_(simple)(ar_handle_t, target) \
	_(count)(int16_t, items) \
	_(dynarray)(TS_ITEM_BASE_INFO, items)

CREATE_PACKET(TS_CS_BUY_FROM_BOOTH, 705);
#undef TS_CS_BUY_FROM_BOOTH_DEF

