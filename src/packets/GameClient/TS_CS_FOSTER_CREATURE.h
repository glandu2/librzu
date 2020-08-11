#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_TICKET_INFO_DEF(_) \
	_(simple)(ar_handle_t, ticket_handle) \
	_(simple)(int32_t, ticket_count)

CREATE_STRUCT(TS_TICKET_INFO);
#undef TS_TICKET_INFO_DEF

#define TS_CRACKER_INFO_DEF(_) \
	_(simple)(ar_handle_t, cracker_handle) \
	_(simple)(int32_t, cracker_count)

CREATE_STRUCT(TS_CRACKER_INFO);
#undef TS_CRACKER_INFO_DEF

#define TS_CS_FOSTER_CREATURE_DEF(_) \
	_(simple)(ar_handle_t, creature_card_handle) \
	_(count)(int32_t, ticket_info) \
	_(count)(int32_t, cracker_info) \
	_(dynarray)(TS_TICKET_INFO, ticket_info) \
	_(dynarray)(TS_CRACKER_INFO, cracker_info)

// Since EPIC_7_3
#define TS_CS_FOSTER_CREATURE_ID(X) \
	X(6002, true)

CREATE_PACKET_VER_ID(TS_CS_FOSTER_CREATURE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_FOSTER_CREATURE_DEF

