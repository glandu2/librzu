#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_REMOVE_SUMMON_INFO_DEF(_) \
	_(simple)(ar_handle_t, card_handle)

CREATE_PACKET(TS_SC_REMOVE_SUMMON_INFO, 302);
#undef TS_SC_REMOVE_SUMMON_INFO_DEF

