#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_EQUIP_SUMMON_DEF(_) \
	_(simple) (bool, open_dialog) \
	_(array) (ar_handle_t, card_handle, 6)

#define TS_EQUIP_SUMMON_ID(X) \
	X(303, version < EPIC_9_6_3) \
	X(1303, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_EQUIP_SUMMON, SessionType::GameClient, SessionPacketOrigin::Any);
#undef TS_EQUIP_SUMMON_DEF

