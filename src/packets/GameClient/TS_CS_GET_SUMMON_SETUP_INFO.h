#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_GET_SUMMON_SETUP_INFO_DEF(_) \
	_(simple)(bool, show_dialog)

#define TS_CS_GET_SUMMON_SETUP_INFO_ID(X) \
	X(324, version < EPIC_9_6_3) \
	X(1324, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_GET_SUMMON_SETUP_INFO, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_GET_SUMMON_SETUP_INFO_DEF

