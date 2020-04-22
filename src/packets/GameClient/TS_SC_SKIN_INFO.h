#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SKIN_INFO_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int32_t, nSkinColor)

CREATE_PACKET(TS_SC_SKIN_INFO, 224);
#undef TS_SC_SKIN_INFO_DEF

