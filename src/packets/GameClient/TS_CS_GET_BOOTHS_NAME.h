#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_GET_BOOTHS_NAME_DEF(_) \
	_(count)(int32_t, handles) \
	_(dynarray)(ar_handle_t, handles)

CREATE_PACKET(TS_CS_GET_BOOTHS_NAME, 707);
#undef TS_CS_GET_BOOTHS_NAME_DEF

