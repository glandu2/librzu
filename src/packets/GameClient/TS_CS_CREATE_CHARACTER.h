#ifndef PACKETS_TS_CS_CREATE_CHARACTER_H
#define PACKETS_TS_CS_CREATE_CHARACTER_H

#include "Packet/PacketDeclaration.h"
#include "TS_SC_CHARACTER_LIST.h"

#define TS_CS_CREATE_CHARACTER_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (LOBBY_CHARACTER_INFO, character)

CREATE_PACKET(TS_CS_CREATE_CHARACTER, 2002);

#endif // PACKETS_TS_CS_CREATE_CHARACTER_H
