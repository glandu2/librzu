#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_HIDE_EQUIP_INFO_DEF(_) \
	_(simple)(ar_handle_t, hPlayer) \
	_(simple)(uint32_t, nHideEquipFlag)

#define TS_SC_HIDE_EQUIP_INFO_ID(X) \
	X(222, version < EPIC_9_6_3) \
	X(1222, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_HIDE_EQUIP_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_HIDE_EQUIP_INFO_DEF

