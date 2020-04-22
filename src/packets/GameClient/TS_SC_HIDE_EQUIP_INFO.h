#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_HIDE_EQUIP_INFO_DEF(_) \
	_(simple)(ar_handle_t, hPlayer) \
	_(simple)(uint32_t, nHideEquipFlag)

CREATE_PACKET(TS_SC_HIDE_EQUIP_INFO, 222);
#undef TS_SC_HIDE_EQUIP_INFO_DEF

