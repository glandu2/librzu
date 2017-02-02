#ifndef PACKETS_TS_CS_VERSION_H
#define PACKETS_TS_CS_VERSION_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_VERSION_DEF(_) \
	_(string)(szVersion, 20)

#define TS_CS_VERSION_ID(X) \
	X(50, version < EPIC_8_1) \
	X(51, version >= EPIC_8_1)

CREATE_PACKET_VER_ID(TS_CS_VERSION);

#endif // PACKETS_TS_CS_VERSION_H
