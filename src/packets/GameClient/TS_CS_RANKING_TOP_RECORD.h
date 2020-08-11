#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_RANKING_TOP_RECORD_DEF(_) \
	_(simple)(int8_t, ranking_type)

#define TS_CS_RANKING_TOP_RECORD_ID(X) \
	X(5000, true)

CREATE_PACKET_VER_ID(TS_CS_RANKING_TOP_RECORD, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_RANKING_TOP_RECORD_DEF

