#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_BOOKMARK_TITLE_DEF(_) \
	_(simple)(int32_t, code)

// Since EPIC_8_1
CREATE_PACKET(TS_CS_BOOKMARK_TITLE, 632);
#undef TS_CS_BOOKMARK_TITLE_DEF

