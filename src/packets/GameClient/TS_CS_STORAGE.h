#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_STORAGE_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(int8_t, mode) \
	_(def)(simple) (int64_t, count) \
	_(impl)(simple)(int64_t, count, version >= EPIC_4_1_1) \
	_(impl)(simple)(uint32_t, count, version < EPIC_4_1_1)

CREATE_PACKET(TS_CS_STORAGE, 212);
#undef TS_CS_STORAGE_DEF

