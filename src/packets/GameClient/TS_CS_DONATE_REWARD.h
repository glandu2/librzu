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

#define TS_CS_DONATE_REWARD_ID(X) \
	X(259, version < EPIC_9_6_3) \
	X(1259, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_DONATE_REWARD, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_DONATE_REWARD_DEF

