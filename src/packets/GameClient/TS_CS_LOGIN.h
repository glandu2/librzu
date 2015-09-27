#ifndef PACKETS_TS_CS_LOGIN_H
#define PACKETS_TS_CS_LOGIN_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_LOGIN_DEF(simple_, array_, dynarray_, count_, string_) \
	string_(name, 19) \
	simple_(uint8_t, race)

CREATE_PACKET(TS_CS_LOGIN, 1);

#endif // PACKETS_TS_CS_LOGIN_H
