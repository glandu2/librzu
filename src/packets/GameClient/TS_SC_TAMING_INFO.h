#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_TAMING_INFO_DEF(_) \
	_(simple)(int8_t, mode) \
	_(simple)(ar_handle_t, tamer_handle) \
	_(simple)(ar_handle_t, target_handle)

CREATE_PACKET(TS_SC_TAMING_INFO, 310);
#undef TS_SC_TAMING_INFO_DEF

