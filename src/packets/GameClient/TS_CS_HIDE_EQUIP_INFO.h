#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_HIDE_EQUIP_INFO_DEF(_) \
	_(simple)(uint32_t, nHideEquipFlag)

#define TS_CS_HIDE_EQUIP_INFO_ID(X) \
	X(221, version < EPIC_9_6_3) \
	X(1221, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_HIDE_EQUIP_INFO, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_HIDE_EQUIP_INFO_DEF

