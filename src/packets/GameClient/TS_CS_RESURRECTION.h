#ifndef PACKETS_TS_CS_RESURRECTION_H
#define PACKETS_TS_CS_RESURRECTION_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_RESURRECTION_DEF(_) \
	_(simple)(uint32_t, handle) \
	_(simple)(int8_t, type)

CREATE_PACKET(TS_CS_RESURRECTION, 513);

#endif // PACKETS_TS_CS_RESURRECTION_H
