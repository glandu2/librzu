#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_RANKING_TOP_RECORD_DEF(_) \
	_(simple)(int8_t, ranking_type)

CREATE_PACKET(TS_CS_RANKING_TOP_RECORD, 5000);
#undef TS_CS_RANKING_TOP_RECORD_DEF

