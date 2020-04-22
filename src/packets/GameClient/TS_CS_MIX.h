#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_MIX_INFO_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(uint16_t, count)

CREATE_STRUCT(TS_MIX_INFO);
#undef TS_MIX_INFO_DEF

#define TS_CS_MIX_DEF(_) \
	_(simple)(TS_MIX_INFO, main_item) \
	_(count)(uint16_t, sub_items) \
	_(dynarray)(TS_MIX_INFO, sub_items)

CREATE_PACKET(TS_CS_MIX, 256);
#undef TS_CS_MIX_DEF

