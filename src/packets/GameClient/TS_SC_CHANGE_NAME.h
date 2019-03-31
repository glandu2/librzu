#ifndef PACKETS_TS_SC_CHANGE_NAME_H
#define PACKETS_TS_SC_CHANGE_NAME_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_CHANGE_NAME_DEF(_) \
	_(simple)(uint32_t, handle) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6)

CREATE_PACKET(TS_SC_CHANGE_NAME, 30);

#endif // PACKETS_TS_SC_CHANGE_NAME_H
