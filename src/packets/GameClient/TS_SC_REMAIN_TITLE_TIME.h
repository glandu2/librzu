#ifndef PACKETS_TS_SC_REMAIN_TITLE_TIME_H
#define PACKETS_TS_SC_REMAIN_TITLE_TIME_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_REMAIN_TITLE_TIME_DEF(_) \
	_(simple)(ar_time_t, remain_title_time)

// Since EPIC_8_1
CREATE_PACKET(TS_SC_REMAIN_TITLE_TIME, 627);
#undef TS_SC_REMAIN_TITLE_TIME_DEF

#endif // PACKETS_TS_SC_REMAIN_TITLE_TIME_H
