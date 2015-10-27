#ifndef PACKETS_TS_CS_QUERY_H
#define PACKETS_TS_CS_QUERY_H

#include "PacketDeclaration.h"

#define TS_CS_QUERY_DEF(simple_, array_, dynarray_, count_) \
	simple_(uint32_t, handle) 

CREATE_PACKET(TS_CS_QUERY, 13);

#endif // PACKETS_TS_CS_QUERY_H