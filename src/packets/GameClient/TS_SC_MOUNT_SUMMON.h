#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_MOUNT_SUMMON_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(ar_handle_t, summon_handle) \
	_(simple)(float, x) \
	_(simple)(float, y) \
	_(simple)(bool, success)

#define TS_SC_MOUNT_SUMMON_ID(X) \
	X(320, version < EPIC_9_6_3) \
	X(1320, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_MOUNT_SUMMON, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_MOUNT_SUMMON_DEF

