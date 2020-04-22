#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_BOOKMARK_TITLE_DEF(_) \
	_(simple)(int32_t, code) \
	_(simple)(bool, bookmarked)

// Since EPIC_8_1
CREATE_PACKET(TS_SC_BOOKMARK_TITLE, 633);
#undef TS_SC_BOOKMARK_TITLE_DEF

