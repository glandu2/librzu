#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_TRANSMIT_ETHEREAL_DURABILITY_DEF(_) \
	_(simple)(ar_handle_t, handle)

// Since EPIC_7_2
#define TS_CS_TRANSMIT_ETHEREAL_DURABILITY_ID(X) \
	X(263, version < EPIC_9_6_3) \
	X(1263, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_TRANSMIT_ETHEREAL_DURABILITY, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_TRANSMIT_ETHEREAL_DURABILITY_DEF

