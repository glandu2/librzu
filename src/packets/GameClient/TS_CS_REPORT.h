#pragma once

#include "Packet/PacketDeclaration.h"

// Null terminator not included in report
// Server support no null terminator (client does not include it)
// Must be sent once only and after version
#define TS_CS_REPORT_DEF(_) \
	_(count)(uint16_t, report) \
	_(dynstring)(report, false)

#define TS_CS_REPORT_ID(X) \
	X(8000, true)

CREATE_PACKET_VER_ID(TS_CS_REPORT, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_REPORT_DEF

