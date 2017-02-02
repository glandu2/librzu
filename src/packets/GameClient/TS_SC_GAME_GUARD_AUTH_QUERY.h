#ifndef PACKETS_TS_SC_GAME_GUARD_AUTH_QUERY_H
#define PACKETS_TS_SC_GAME_GUARD_AUTH_QUERY_H

#include "Packet/PacketDeclaration.h"
  
#define TS_SC_GAME_GUARD_AUTH_QUERY_DEF(_) \
	_(simple)(uint32_t, dwIndex) \
	_(simple)(uint32_t, dwValue1) \
	_(simple)(uint32_t, dwValue2) \
	_(simple)(uint32_t, dwValue3)

CREATE_PACKET(TS_SC_GAME_GUARD_AUTH_QUERY, 55);

#endif // PACKETS_TS_SC_GAME_GUARD_AUTH_QUERY_H
