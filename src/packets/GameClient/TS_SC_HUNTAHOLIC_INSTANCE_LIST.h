#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_HUNTAHOLIC_INSTANCE_INFO.h"

#define TS_SC_HUNTAHOLIC_INSTANCE_LIST_DEF(_) \
	_(simple)(int32_t, huntaholic_id) \
	_(simple)(int32_t, page) \
	_(count)(int32_t, infos) \
	_(simple)(int32_t, total_page) \
	_(dynarray)(TS_HUNTAHOLIC_INSTANCE_INFO, infos)

#define TS_SC_HUNTAHOLIC_INSTANCE_LIST_ID(X) \
	X(4001, true)

CREATE_PACKET_VER_ID(TS_SC_HUNTAHOLIC_INSTANCE_LIST, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_HUNTAHOLIC_INSTANCE_LIST_DEF

