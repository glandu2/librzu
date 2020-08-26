#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_AC_ACCOUNT_NAME_DEF(_) \
	_(string)(account, 61) \
	_(simple)(uint32_t, account_id) \
	_(simple)(uint32_t, unknown1, version >= EPIC_9_6_3) \
	_(string)(unknown2_hex, 57, version >= EPIC_9_6_3) \
	_(simple)(uint16_t, unknown3, version >= EPIC_9_6_3) \
	_(simple)(uint8_t, unknown4, version >= EPIC_9_6_3) \
	_(simple)(uint32_t, unknown5, version >= EPIC_9_6_3) \
	_(simple)(uint32_t, unknown6, version >= EPIC_9_6_3) \
	_(simple)(uint32_t, unknown7, version >= EPIC_9_6_3) \
	_(simple)(uint32_t, unknown8, version >= EPIC_9_6_3) \
	_(simple)(uint32_t, unknown9, version >= EPIC_9_6_3) \
	_(simple)(uint32_t, unknown10, version >= EPIC_9_6_3) \
	_(simple)(uint32_t, unknown11, version >= EPIC_9_6_3) \
	_(simple)(uint32_t, unknown12, version >= EPIC_9_6_3) \
	_(simple)(uint32_t, unknown13, version >= EPIC_9_6_3)

CREATE_PACKET(TS_AC_ACCOUNT_NAME, 10014, SessionType::AuthClient, SessionPacketOrigin::Server);

