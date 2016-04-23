#ifndef PACKETS_TS_CS_REGION_UPDATE_H
#define PACKETS_TS_CS_REGION_UPDATE_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_REGION_UPDATE_DEF(_) \
	_(simple)(uint32_t, update_time) \
	_(simple)(float, x) \
	_(simple)(float, y) \
	_(simple)(float, z) \
	_(simple)(bool, bIsStopMessage)

CREATE_PACKET(TS_CS_REGION_UPDATE, 7);

#endif // PACKETS_TS_CS_REGION_UPDATE_H
