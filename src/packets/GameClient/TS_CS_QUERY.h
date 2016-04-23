#ifndef PACKETS_TS_CS_QUERY_H
#define PACKETS_TS_CS_QUERY_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_QUERY_DEF(_) \
	_(simple)(uint32_t, handle) 

CREATE_PACKET(TS_CS_QUERY, 13);

#endif // PACKETS_TS_CS_QUERY_H
