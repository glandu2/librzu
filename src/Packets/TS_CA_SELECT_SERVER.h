#ifndef PACKETS_TS_CA_SELECT_SERVER_H
#define PACKETS_TS_CA_SELECT_SERVER_H

#include "PacketDeclaration.h"

#define TS_CA_SELECT_SERVER_DEF(simple_, array_, dynarray_, count_) \
	simple_(uint16_t, server_idx)

CREATE_PACKET(TS_CA_SELECT_SERVER, 10023);

#endif // PACKETS_TS_CA_SELECT_SERVER_H
