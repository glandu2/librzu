#ifndef PACKETS_TS_CS_CHARACTER_LIST_H
#define PACKETS_TS_CS_CHARACTER_LIST_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHARACTER_LIST_DEF(_) \
	_(string)(account, 61)

#define TS_CS_CHARACTER_LIST_ID(X) \
	X(2001, version < EPIC_9_4) \
	X(2007, version >= EPIC_9_4)

CREATE_PACKET_VER_ID(TS_CS_CHARACTER_LIST);

#endif // PACKETS_TS_CS_CHARACTER_LIST_H
