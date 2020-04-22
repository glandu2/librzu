#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_HUNTAHOLIC_CREATE_INSTANCE_DEF(_) \
	_(string)(name, 31) \
	_(simple)(int8_t, max_member_count) \
	_(string)(password, 17)

CREATE_PACKET(TS_CS_HUNTAHOLIC_CREATE_INSTANCE, 4003);
#undef TS_CS_HUNTAHOLIC_CREATE_INSTANCE_DEF

