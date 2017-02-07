#ifndef PACKETS_TS_CS_LEARN_SKILL_H
#define PACKETS_TS_CS_LEARN_SKILL_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_LEARN_SKILL_DEF(_) \
	_(simple)(uint32_t, target) \
	_(simple)(int32_t, skill_id) \
	_(simple)(int16_t, skill_level)

CREATE_PACKET(TS_CS_LEARN_SKILL, 402);

#endif // PACKETS_TS_CS_LEARN_SKILL_H
