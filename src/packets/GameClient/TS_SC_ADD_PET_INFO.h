#ifndef PACKETS_TS_SC_ADD_PET_INFO_H
#define PACKETS_TS_SC_ADD_PET_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_ADD_PET_INFO_DEF(_) \
	_(simple)(ar_handle_t, cage_handle) \
	_(simple)(ar_handle_t, pet_handle) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6) \
	_(simple)(int32_t, code)

CREATE_PACKET(TS_SC_ADD_PET_INFO, 351);
#undef TS_SC_ADD_PET_INFO_DEF

#endif // PACKETS_TS_SC_ADD_PET_INFO_H
