#ifndef PACKETS_TS_CA_VERSION_H
#define PACKETS_TS_CA_VERSION_H

#include "Packet/PacketDeclaration.h"

#define TS_CA_VERSION_DEF(_) \
	_(string)(szVersion, 20)
CREATE_PACKET(TS_CA_VERSION, 10001);

#endif // PACKETS_TS_CA_VERSION_H
