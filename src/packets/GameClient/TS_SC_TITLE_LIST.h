#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_TITLE_INFO_DEF(_) \
	_(simple)(int32_t, code) \
	_(simple)(int32_t, status)

CREATE_STRUCT(TS_TITLE_INFO);
#undef TS_TITLE_INFO_DEF

#define TS_SC_TITLE_LIST_DEF(_) \
	_(count)(uint16_t, titles) \
	_(dynarray)(TS_TITLE_INFO, titles)

// Since EPIC_8_1
CREATE_PACKET(TS_SC_TITLE_LIST, 625);
#undef TS_SC_TITLE_LIST_DEF

