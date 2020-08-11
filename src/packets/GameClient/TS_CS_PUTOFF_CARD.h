#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_PUTOFF_CARD_DEF(_) \
	_(simple)(int8_t, position)

#define TS_CS_PUTOFF_CARD_ID(X) \
	X(215, version < EPIC_9_6_3) \
	X(1215, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_PUTOFF_CARD, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_PUTOFF_CARD_DEF

