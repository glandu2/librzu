#ifndef PACKETS_TS_SC_CLEAR_SECURITY_NO_H
#define PACKETS_TS_SC_CLEAR_SECURITY_NO_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_CLEAR_SECURITY_NO_DEF(_) \
	_(simple)(bool, clearable) \

CREATE_PACKET(TS_SC_CLEAR_SECURITY_NO, 9011);

#endif // PACKETS_TS_SC_CLEAR_SECURITY_NO_H
