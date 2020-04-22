#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_CHARACTER_LIST.h"

#define TS_CS_CREATE_CHARACTER_DEF(_) \
	_(simple) (LOBBY_CHARACTER_INFO, character)

CREATE_PACKET(TS_CS_CREATE_CHARACTER, 2002);
#undef TS_CS_CREATE_CHARACTER_DEF

