#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_DROP_RESULT_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(bool, isAccepted)

CREATE_PACKET(TS_SC_DROP_RESULT, 205);
#undef TS_SC_DROP_RESULT_DEF

