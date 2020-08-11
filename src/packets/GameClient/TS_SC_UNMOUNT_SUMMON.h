#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_UNMOUNT_SUMMON_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(ar_handle_t, summon_handle) \
	_(simple)(int8_t, flag)

#define TS_SC_UNMOUNT_SUMMON_ID(X) \
	X(321, version < EPIC_9_6_3) \
	X(1321, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_UNMOUNT_SUMMON, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_UNMOUNT_SUMMON_DEF

