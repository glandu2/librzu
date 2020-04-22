#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_REQUEST_REMOVE_STATE_DEF(_) \
	_(simple)(ar_handle_t, target) \
	_(simple)(int32_t, state_code)

CREATE_PACKET(TS_CS_REQUEST_REMOVE_STATE, 408);
#undef TS_CS_REQUEST_REMOVE_STATE_DEF

