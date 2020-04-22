#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_ARRANGE_ITEM_DEF(_) \
	_(simple)(bool, bIsStorage)

CREATE_PACKET(TS_CS_ARRANGE_ITEM, 219);
#undef TS_CS_ARRANGE_ITEM_DEF

