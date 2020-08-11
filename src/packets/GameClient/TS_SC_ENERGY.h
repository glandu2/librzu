#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_ENERGY_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int16_t, energy)

#define TS_SC_ENERGY_ID(X) \
	X(515, version < EPIC_9_6_3) \
	X(1515, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_ENERGY, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_ENERGY_DEF

