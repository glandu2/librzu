#ifndef PACKETS_TS_SC_CHANGE_NAME_H
#define PACKETS_TS_SC_CHANGE_NAME_H

#include "PacketDeclaration.h"

#define TS_SC_CHANGE_NAME_DEF(simple_, array_, dynarray_, count_) \
	simple_(uint32_t, handle) \
	array_(char, 19, name)

CREATE_PACKET(TS_SC_CHANGE_NAME, 30);

#endif // PACKETS_TS_SC_CHANGE_NAME_H