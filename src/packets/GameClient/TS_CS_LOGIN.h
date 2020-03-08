#ifndef PACKETS_TS_CS_LOGIN_H
#define PACKETS_TS_CS_LOGIN_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_LOGIN_DEF(_) \
	_(def)(string)(name, 24) \
	_(impl)(string)(name, 24, version >= EPIC_9_3) \
	_(impl)(string)(name, 19, version < EPIC_9_3) \
	_(simple)(uint8_t, race, version < EPIC_9_3, 0) \
	_(array)(uint8_t, checksum, 256, version >= EPIC_9_5_2)

#define TS_CS_LOGIN_ID(X) \
	X(1, version < EPIC_9_2) \
	X(61, version >= EPIC_9_2 && version < EPIC_9_4_2) \
	X(62, version >= EPIC_9_4_2)

CREATE_PACKET_VER_ID(TS_CS_LOGIN);

#endif // PACKETS_TS_CS_LOGIN_H
