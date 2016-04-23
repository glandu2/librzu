#ifndef PACKETS_TS_CS_LOGIN_H
#define PACKETS_TS_CS_LOGIN_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_LOGIN_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	string_(name, 19) \
	simple_(char, race)

#define TS_CS_LOGIN_ID(X) \
	X(1, version < EPIC_9_2) \
	X(61, version >= EPIC_9_2)

CREATE_PACKET_VER_ID(TS_CS_LOGIN);

#endif // PACKETS_TS_CS_LOGIN_H
