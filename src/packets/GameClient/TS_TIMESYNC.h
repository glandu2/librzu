#ifndef PACKETS_TS_TIMESYNC_H
#define PACKETS_TS_TIMESYNC_H

#include "Packet/PacketDeclaration.h"

#define TS_TIMESYNC_DEF(_) \
	_(simple) (ar_time_t, time)

CREATE_PACKET(TS_TIMESYNC, 2);
#undef TS_TIMESYNC_DEF

#endif // PACKETS_TS_TIMESYNC_H
