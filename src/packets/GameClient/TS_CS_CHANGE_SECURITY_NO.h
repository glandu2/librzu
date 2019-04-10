#ifndef PACKETS_TS_CS_CHANGE_SECURITY_NO_H
#define PACKETS_TS_CS_CHANGE_SECURITY_NO_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHANGE_SECURITY_NO_DEF(_) \
	_(array)(uint8_t, security_no_old, 19) \
	_(array)(uint8_t, security_no, 19)

CREATE_PACKET(TS_CS_CHANGE_SECURITY_NO, 9007);

#endif // PACKETS_TS_CS_CHANGE_SECURITY_NO_H
