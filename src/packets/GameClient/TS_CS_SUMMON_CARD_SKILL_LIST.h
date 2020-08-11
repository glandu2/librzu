#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_SUMMON_CARD_SKILL_LIST_DEF(_) \
	_(simple)(ar_handle_t, item_handle)

// Since EPIC_7_3
#define TS_CS_SUMMON_CARD_SKILL_LIST_ID(X) \
	X(452, version < EPIC_9_6_3) \
	X(1452, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_SUMMON_CARD_SKILL_LIST, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_SUMMON_CARD_SKILL_LIST_DEF

