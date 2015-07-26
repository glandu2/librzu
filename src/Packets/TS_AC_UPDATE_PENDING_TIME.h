#ifndef PACKETS_TS_AC_UPDATE_PENDING_TIME_H
#define PACKETS_TS_AC_UPDATE_PENDING_TIME_H

#include "PacketDeclaration.h"

//Not used in 6.1 server
#define TS_AC_UPDATE_PENDING_TIME_DEF(simple_, array_, dynarray_, count_) \
	simple_(uint32_t, pending_time)

CREATE_PACKET(TS_AC_UPDATE_PENDING_TIME, 10025); //id not sure

#endif // PACKETS_TS_AC_UPDATE_PENDING_TIME_H
