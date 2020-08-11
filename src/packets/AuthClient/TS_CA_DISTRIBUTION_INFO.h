#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CA_DISTRIBUTION_INFO_DEF(_) \
	_(endstring)(distributionInfo, true)
CREATE_PACKET(TS_CA_DISTRIBUTION_INFO, 10026, SessionType::AuthClient, SessionPacketOrigin::Client);

