#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_GET_REGION_INFO_DEF(_) \
	_(simple)(float, x) \
	_(simple)(float, y)

CREATE_PACKET(TS_CS_GET_REGION_INFO, 550);
#undef TS_CS_GET_REGION_INFO_DEF

