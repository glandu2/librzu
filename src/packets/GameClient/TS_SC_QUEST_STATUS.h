#ifndef PACKETS_TS_SC_QUEST_STATUS_H
#define PACKETS_TS_SC_QUEST_STATUS_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_QUEST_STATUS_DEF(_) \
	_(simple)(int32_t, code) \
	_(array)(int32_t, nStatus, 6) \
	_(simple)(int8_t, nProgress) \
	_(simple)(uint32_t, nTimeLimit)

CREATE_PACKET(TS_SC_QUEST_STATUS, 601);

#endif // PACKETS_TS_SC_QUEST_STATUS_H
