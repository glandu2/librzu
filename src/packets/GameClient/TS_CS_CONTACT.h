#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_CONTACT_DEF(_) \
	_(simple)(ar_handle_t, handle)

CREATE_PACKET(TS_CS_CONTACT, 3002);
#undef TS_CS_CONTACT_DEF

