#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_PUTOFF_ITEM_DEF(_) \
	_(simple) (int8_t, position) \
	_(simple) (ar_handle_t, target_handle)

CREATE_PACKET(TS_CS_PUTOFF_ITEM, 201);
#undef TS_CS_PUTOFF_ITEM_DEF

