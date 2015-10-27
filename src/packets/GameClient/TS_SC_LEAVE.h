#ifndef PACKETS_TS_SC_LEAVE_H
#define PACKETS_TS_SC_LEAVE_H

#include "PacketDeclaration.h"

#define TS_SC_LEAVE_DEF(simple_, array_, dynarray_, count_) \
	simple_(uint32_t, handle)

CREATE_PACKET(TS_SC_LEAVE, 9);

#endif // PACKETS_TS_SC_LEAVE_H