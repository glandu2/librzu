#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_LEVEL_UPDATE_DEF(_) \
	_(simple) (ar_handle_t, handle) \
	_(simple) (int32_t, level) \
	_(simple) (int32_t, job_level)

CREATE_PACKET(TS_SC_LEVEL_UPDATE, 1002);
#undef TS_SC_LEVEL_UPDATE_DEF

