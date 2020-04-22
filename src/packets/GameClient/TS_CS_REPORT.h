#pragma once

#include "Packet/PacketDeclaration.h"

// Null terminator not included in report
// Server support no null terminator (client does not include it)
// Must be sent once only and after version
#define TS_CS_REPORT_DEF(_) \
	_(count)(uint16_t, report) \
	_(dynstring)(report, false)

CREATE_PACKET(TS_CS_REPORT, 8000);
#undef TS_CS_REPORT_DEF

