#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_REWARD_INFO_DEF(_) \
	_(simple)(int8_t, reward_type) \
	_(simple)(uint16_t, count)

CREATE_STRUCT(TS_REWARD_INFO);
#undef TS_REWARD_INFO_DEF

#define TS_CS_DONATE_REWARD_DEF(_) \
	_(count)(int8_t, rewards) \
	_(dynarray)(TS_REWARD_INFO, rewards)

CREATE_PACKET(TS_CS_DONATE_REWARD, 259);
#undef TS_CS_DONATE_REWARD_DEF

