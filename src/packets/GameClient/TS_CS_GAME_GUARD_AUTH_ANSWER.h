#ifndef PACKETS_TS_CS_GAME_GUARD_AUTH_ANSWER_H
#define PACKETS_TS_CS_GAME_GUARD_AUTH_ANSWER_H

#include "Packet/PacketDeclaration.h"
  
#define TS_CS_GAME_GUARD_AUTH_ANSWER_DEF(_) \
	_(simple)(uint32_t, dwIndex) \
	_(simple)(uint32_t, dwValue1) \
	_(simple)(uint32_t, dwValue2) \
	_(simple)(uint32_t, dwValue3)

CREATE_PACKET(TS_CS_GAME_GUARD_AUTH_ANSWER, 56);

#endif // PACKETS_TS_CS_GAME_GUARD_AUTH_ANSWER_H
