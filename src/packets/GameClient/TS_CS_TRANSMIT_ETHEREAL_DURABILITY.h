#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_TRANSMIT_ETHEREAL_DURABILITY_DEF(_) \
	_(simple)(ar_handle_t, handle)

// Since EPIC_7_2
CREATE_PACKET(TS_CS_TRANSMIT_ETHEREAL_DURABILITY, 263);
#undef TS_CS_TRANSMIT_ETHEREAL_DURABILITY_DEF

