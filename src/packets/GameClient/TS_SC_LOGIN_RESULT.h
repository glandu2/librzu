#pragma once

#include "Packet/PacketDeclaration.h"
#include "PacketEnums.h"

#define TS_SC_LOGIN_RESULT_DEF(_) \
	_(def)(simple)(uint16_t, result) \
	  _(impl)(simple)(uint16_t, result, version >= EPIC_7_1) \
	  _(impl)(simple)(uint8_t, result, version <  EPIC_7_1) \
	_(simple) (ar_handle_t, handle) \
	_(simple) (float, x) \
	_(simple) (float, y) \
	_(simple) (float, z) \
	_(def)(simple) (uint32_t, layer) \
	  _(impl)(simple)(uint32_t, layer, version >= EPIC_9_6_2) \
	  _(impl)(simple)(uint8_t, layer, version < EPIC_9_6_2) \
	_(simple) (float, face_direction) \
	_(simple) (int32_t, region_size) \
	_(simple) (int32_t, hp) \
	_(def)(simple)(int32_t, mp) \
	_(impl)(simple)(int32_t, mp, version >= EPIC_7_3) \
	_(impl)(simple)(int16_t, mp, version <  EPIC_7_3) \
	_(simple) (int32_t, max_hp) \
	_(def)(simple)(int32_t, max_mp) \
	  _(impl)(simple)(int32_t, max_mp, version >= EPIC_7_3) \
	  _(impl)(simple)(int16_t, max_mp, version <  EPIC_7_3) \
	_(simple) (int32_t, havoc, version >= EPIC_4_1 && version < EPIC_8_2) \
	_(simple) (int32_t, max_havoc, version >= EPIC_4_1 && version < EPIC_8_2) \
	_(simple) (int32_t, sex) \
	_(simple) (int32_t, race) \
	_(simple) (uint32_t, skin_color, version >= EPIC_4_1) \
	_(simple) (int32_t, faceId) \
	_(simple) (int32_t, hairId) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6) \
	_(simple) (int32_t, cell_size) \
	_(simple) (int32_t, guild_id) \
	_(simple) (int32_t, back_board, version >= EPIC_9_2) \
	_(simple) (int32_t, unknown_2, version >= EPIC_9_6_2)

#define TS_SC_LOGIN_RESULT_ID(X) \
	X(4, version < EPIC_9_2) \
	X(64, version >= EPIC_9_2 && version < EPIC_9_6_3) \
	X(1064, version >= EPIC_9_6_3) \

CREATE_PACKET_VER_ID(TS_SC_LOGIN_RESULT, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_LOGIN_RESULT_DEF

