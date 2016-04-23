#ifndef PACKETS_TS_SC_LOGIN_RESULT_H
#define PACKETS_TS_SC_LOGIN_RESULT_H

#include "Packet/PacketDeclaration.h"
#include "PacketEnums.h"

#define TS_SC_LOGIN_RESULT_DEF(_) \
	_(simple)(def)(uint16_t, result) \
	_(simple)(impl)(uint16_t, result, version >= EPIC_7_1) \
	_(simple)(impl)(uint8_t, result, version <  EPIC_7_1) \
	_(simple) (uint32_t, handle) \
	_(simple) (float, x) \
	_(simple) (float, y) \
	_(simple) (float, z) \
	_(simple) (uint8_t, layer) \
	_(simple) (float, face_direction) \
	_(simple) (int32_t, region_size) \
	_(simple) (int32_t, hp) \
	_(simple)(def)(int32_t, mp) \
	_(simple)(impl)(int32_t, mp, version >= EPIC_7_1) \
	_(simple)(impl)(int16_t, mp, version <  EPIC_7_1) \
	_(simple) (int32_t, max_hp) \
	_(simple)(def)(int32_t, max_mp) \
	_(simple)(impl)(int32_t, max_mp, version >= EPIC_7_1) \
	_(simple)(impl)(int16_t, max_mp, version <  EPIC_7_1) \
	_(simple) (int32_t, havoc, version >= EPIC_4_1 && version < EPIC_9_2) \
	_(simple) (int32_t, max_havoc, version >= EPIC_4_1 && version < EPIC_9_2) \
	_(simple) (int32_t, sex) \
	_(simple) (int32_t, race) \
	_(simple) (uint32_t, skin_color, version >= EPIC_4_1) \
	_(simple) (int32_t, faceId) \
	_(simple) (int32_t, hairId) \
	_(string) (name, 19) \
	_(simple) (int32_t, cell_size) \
	_(simple) (int32_t, guild_id, version >= EPIC_5_1) \
	_(simple) (int32_t, unknown, version >= EPIC_9_2)

#define TS_SC_LOGIN_RESULT_ID(X) \
	X(4, version < EPIC_9_2) \
	X(64, version >= EPIC_9_2)

CREATE_PACKET_VER_ID(TS_SC_LOGIN_RESULT);

#endif // PACKETS_TS_SC_LOGIN_RESULT_H
