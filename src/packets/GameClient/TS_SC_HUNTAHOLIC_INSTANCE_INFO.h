#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_HUNTAHOLIC_INSTANCE_INFO_DEF(_) \
	_(simple)(int32_t, instance_no) \
	_(string)(name, 31) \
	_(simple)(int8_t, current_member_count) \
	_(simple)(int8_t, max_member_count) \
	_(simple)(bool, require_password)

CREATE_STRUCT(TS_HUNTAHOLIC_INSTANCE_INFO);
#undef TS_HUNTAHOLIC_INSTANCE_INFO_DEF

#define TS_SC_HUNTAHOLIC_INSTANCE_INFO_DEF(_) \
	_(simple)(TS_HUNTAHOLIC_INSTANCE_INFO, info)

#define TS_SC_HUNTAHOLIC_INSTANCE_INFO_ID(X) \
	X(4002, true)

CREATE_PACKET_VER_ID(TS_SC_HUNTAHOLIC_INSTANCE_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_HUNTAHOLIC_INSTANCE_INFO_DEF

