#ifndef PACKETS_TS_CS_DELETE_CHARACTER_H
#define PACKETS_TS_CS_DELETE_CHARACTER_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_DELETE_CHARACTER_DEF(simple_, array_, dynarray_, count_, string_) \
	string_(name, 19)

CREATE_PACKET(TS_CS_DELETE_CHARACTER, 2003);

#endif // PACKETS_TS_CS_DELETE_CHARACTER_H
