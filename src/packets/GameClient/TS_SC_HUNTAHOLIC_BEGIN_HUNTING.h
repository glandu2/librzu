#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_HUNTAHOLIC_BEGIN_HUNTING_DEF(_) \
	_(simple)(ar_time_t, begin_time)

CREATE_PACKET(TS_SC_HUNTAHOLIC_BEGIN_HUNTING, 4009);
#undef TS_SC_HUNTAHOLIC_BEGIN_HUNTING_DEF

