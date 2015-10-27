#ifndef PACKETS_TS_SC_LOGIN_RESULT_H
#define PACKETS_TS_SC_LOGIN_RESULT_H

#include "Packet/PacketDeclaration.h"
#include "PacketEnums.h"

#define TS_SC_LOGIN_RESULT_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint16_t, result) \
	simple_ (uint32_t, handle) \
	simple_ (float, x) \
	simple_ (float, y) \
	simple_ (float, z) \
	simple_ (uint8_t, layer) \
	simple_ (float, face_direction) \
	simple_ (int32_t, region_size) \
	simple_ (int32_t, hp) \
	simple_ (int32_t, mp) \
	simple_ (int32_t, max_hp) \
	simple_ (int32_t, max_mp) \
	simple_ (int32_t, havoc) \
	simple_ (int32_t, max_havoc) \
	simple_ (int32_t, sex) \
	simple_ (int32_t, race) \
	simple_ (uint32_t, skin_color) \
	simple_ (int32_t, faceId) \
	simple_ (int32_t, hairId) \
	string_ (name, 19) \
	simple_ (int32_t, cell_size) \
	simple_ (int32_t, guild_id)

CREATE_PACKET(TS_SC_LOGIN_RESULT, 4);

#endif // PACKETS_TS_SC_LOGIN_RESULT_H
