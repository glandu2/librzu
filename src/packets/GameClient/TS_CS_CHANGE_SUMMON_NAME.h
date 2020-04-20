#ifndef PACKETS_TS_CS_CHANGE_SUMMON_NAME_H
#define PACKETS_TS_CS_CHANGE_SUMMON_NAME_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHANGE_SUMMON_NAME_DEF(_) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6)

CREATE_PACKET(TS_CS_CHANGE_SUMMON_NAME, 323);
#undef TS_CS_CHANGE_SUMMON_NAME_DEF

#endif // PACKETS_TS_CS_CHANGE_SUMMON_NAME_H
