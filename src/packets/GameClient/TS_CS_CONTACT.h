#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_CONTACT_DEF(_) \
	_(simple)(ar_handle_t, handle)

#define TS_CS_CONTACT_ID(X) \
	X(3002, true)

CREATE_PACKET_VER_ID(TS_CS_CONTACT, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_CONTACT_DEF

