#ifndef PACKETS_TS_CS_STORAGE_H
#define PACKETS_TS_CS_STORAGE_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_STORAGE_DEF(_) \
	_(simple)(uint32_t, item_handle) \
	_(simple)(int8_t, mode) \
	_(simple)(int64_t, count)

CREATE_PACKET(TS_CS_STORAGE, 212);

#endif // PACKETS_TS_CS_STORAGE_H
