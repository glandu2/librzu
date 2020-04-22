#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_UPDATE_DEF(_) \
	_(simple) (ar_handle_t, handle) \
	_(simple) (ar_time_t, time, version >= EPIC_9_2, static_cast<ar_time_t>(0)) \
	_(simple) (uint32_t, epoch_time, version >= EPIC_9_2, 0)

CREATE_PACKET(TS_CS_UPDATE, 503);
#undef TS_CS_UPDATE_DEF

