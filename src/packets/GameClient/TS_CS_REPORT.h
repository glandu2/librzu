#ifndef PACKETS_TS_CS_REPORT_H
#define PACKETS_TS_CS_REPORT_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_REPORT_DEF(_) \
	_(count)(uint16_t, report_size, report) \
	_(dynstring)(report, false)

CREATE_PACKET(TS_CS_REPORT, 8000);

#endif // PACKETS_TS_CS_REPORT_H
