#ifndef PACKETS_TS_CS_REQUEST_H
#define PACKETS_TS_CS_REQUEST_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_REQUEST_DEF(_) \
	_(simple)(uint8_t, t) \
	_(endstring)(command, true)

CREATE_PACKET(TS_CS_REQUEST, 60);
#undef TS_CS_REQUEST_DEF

#endif // PACKETS_TS_CS_REQUEST_H
