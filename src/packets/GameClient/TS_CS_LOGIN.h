#ifndef PACKETS_TS_CS_LOGIN_H
#define PACKETS_TS_CS_LOGIN_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_LOGIN_DEF(_) \
	_(string)(name, 19) \
	_(simple)(uint8_t, race) \
	_(simple)(uint32_t, unknown, version >= EPIC_9_3, 0)

#define TS_CS_LOGIN_ID(X) \
	X(1, version < EPIC_9_2) \
	X(61, version >= EPIC_9_2 && version < EPIC_9_4) \
	X(62, version >= EPIC_9_4)

CREATE_PACKET_VER_ID(TS_CS_LOGIN);

#endif // PACKETS_TS_CS_LOGIN_H
