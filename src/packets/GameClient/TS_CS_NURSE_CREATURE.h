#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_NURSE_CREATURE_DEF(_) \
	_(simple)(ar_handle_t, creature_card_handle)

// Since EPIC_7_3
#define TS_CS_NURSE_CREATURE_ID(X) \
	X(6006, true)

CREATE_PACKET_VER_ID(TS_CS_NURSE_CREATURE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_NURSE_CREATURE_DEF

