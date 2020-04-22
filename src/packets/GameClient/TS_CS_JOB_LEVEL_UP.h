#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_JOB_LEVEL_UP_DEF(_) \
	_(simple)(ar_handle_t, target)

CREATE_PACKET(TS_CS_JOB_LEVEL_UP, 410);
#undef TS_CS_JOB_LEVEL_UP_DEF

