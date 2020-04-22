#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_COMMERCIAL_STORAGE_INFO_DEF(_) \
	_(simple) (uint16_t, total_item_count) \
	_(simple) (uint16_t, new_item_count)

CREATE_PACKET(TS_SC_COMMERCIAL_STORAGE_INFO, 10003);
#undef TS_SC_COMMERCIAL_STORAGE_INFO_DEF

