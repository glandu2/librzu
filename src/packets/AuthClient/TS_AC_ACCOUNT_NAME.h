#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_AC_ACCOUNT_NAME_DEF(_) \
	_(string)(account, 61) \
	_(simple)(uint32_t, account_id)
CREATE_PACKET(TS_AC_ACCOUNT_NAME, 10014);

