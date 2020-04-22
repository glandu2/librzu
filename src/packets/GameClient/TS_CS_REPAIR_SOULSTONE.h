#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_REPAIR_SOULSTONE_DEF(_) \
	_(array)(ar_handle_t, item_handle, 6)

CREATE_PACKET(TS_CS_REPAIR_SOULSTONE, 262);
#undef TS_CS_REPAIR_SOULSTONE_DEF

