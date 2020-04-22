#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_ITEM_KEEPING_TAKE_DEF(_) \
	_(simple)(int32_t, keeping_uid)

CREATE_PACKET(TS_CS_ITEM_KEEPING_TAKE, 1352);
#undef TS_CS_ITEM_KEEPING_TAKE_DEF

