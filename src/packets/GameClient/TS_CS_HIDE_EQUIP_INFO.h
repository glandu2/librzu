#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_HIDE_EQUIP_INFO_DEF(_) \
	_(simple)(uint32_t, nHideEquipFlag)

CREATE_PACKET(TS_CS_HIDE_EQUIP_INFO, 221);
#undef TS_CS_HIDE_EQUIP_INFO_DEF

