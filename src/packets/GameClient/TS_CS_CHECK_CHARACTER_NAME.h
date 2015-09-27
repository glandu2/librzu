#ifndef PACKETS_TS_CS_CHECK_CHARACTER_NAME_H
#define PACKETS_TS_CS_CHECK_CHARACTER_NAME_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHECK_CHARACTER_NAME_DEF(simple_, array_, dynarray_, count_, string_) \
	string_(name, 19)

CREATE_PACKET(TS_CS_CHECK_CHARACTER_NAME, 2006);

#endif // PACKETS_TS_CS_CHECK_CHARACTER_NAME_H
