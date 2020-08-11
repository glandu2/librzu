#pragma once

#include "Packet/PacketDeclaration.h"

// Seems unused
#define TS_CS_SUMMON_DEF(_) \
	_(simple)(int8_t, is_summon) \
	_(simple)(ar_handle_t, card_handle)

#define TS_CS_SUMMON_ID(X) \
	X(304, version < EPIC_9_6_3) \
	X(1304, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_SUMMON, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_SUMMON_DEF

