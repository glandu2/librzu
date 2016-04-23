#ifndef PACKETS_TS_SC_LOGIN_RESULT_H
#define PACKETS_TS_SC_LOGIN_RESULT_H

#include "Packet/PacketDeclaration.h"
#include "PacketEnums.h"

#define TS_SC_LOGIN_RESULT_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_(def)(uint16_t, result) \
	simple_(impl)(uint16_t, result, version >= EPIC_7_1) \
	simple_(impl)(uint8_t, result, version <  EPIC_7_1) \
	simple_ (uint32_t, handle) \
	simple_ (float, x) \
	simple_ (float, y) \
	simple_ (float, z) \
	simple_ (uint8_t, layer) \
	simple_ (float, face_direction) \
	simple_ (int32_t, region_size) \
	simple_ (int32_t, hp) \
	simple_(def)(int32_t, mp) \
	simple_(impl)(int32_t, mp, version >= EPIC_7_1) \
	simple_(impl)(int16_t, mp, version <  EPIC_7_1) \
	simple_ (int32_t, max_hp) \
	simple_(def)(int32_t, max_mp) \
	simple_(impl)(int32_t, max_mp, version >= EPIC_7_1) \
	simple_(impl)(int16_t, max_mp, version <  EPIC_7_1) \
	simple_ (int32_t, havoc, version >= EPIC_4_1 && version < EPIC_9_2) \
	simple_ (int32_t, max_havoc, version >= EPIC_4_1 && version < EPIC_9_2) \
	simple_ (int32_t, sex) \
	simple_ (int32_t, race) \
	simple_ (uint32_t, skin_color, version >= EPIC_4_1) \
	simple_ (int32_t, faceId) \
	simple_ (int32_t, hairId) \
	string_ (name, 19) \
	simple_ (int32_t, cell_size) \
	simple_ (int32_t, guild_id, version >= EPIC_5_1) \
	simple_ (int32_t, unknown, version >= EPIC_9_2)

#define TS_SC_LOGIN_RESULT_ID(X) \
	X(4, version < EPIC_9_2) \
	X(64, version >= EPIC_9_2)

CREATE_PACKET_VER_ID(TS_SC_LOGIN_RESULT);

#endif // PACKETS_TS_SC_LOGIN_RESULT_H
