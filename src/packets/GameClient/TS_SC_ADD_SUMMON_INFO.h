#ifndef PACKETS_TS_SC_ADD_SUMMON_INFO_H
#define PACKETS_TS_SC_ADD_SUMMON_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_ADD_SUMMON_INFO_DEF(_) \
	_(simple)(ar_handle_t, card_handle) \
	_(simple)(ar_handle_t, summon_handle) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6) \
	_(simple)(int32_t, code) \
	_(simple)(int32_t, level) \
	_(simple)(int32_t, sp)

CREATE_PACKET(TS_SC_ADD_SUMMON_INFO, 301);
#undef TS_SC_ADD_SUMMON_INFO_DEF

#endif // PACKETS_TS_SC_ADD_SUMMON_INFO_H
