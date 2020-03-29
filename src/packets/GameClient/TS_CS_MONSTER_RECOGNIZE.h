#ifndef PACKETS_TS_CS_MONSTER_RECOGNIZE_H
#define PACKETS_TS_CS_MONSTER_RECOGNIZE_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_MONSTER_RECOGNIZE_DEF(_) \
	_(simple)(ar_handle_t, recognizer_handle) \
	_(simple)(ar_handle_t, monster_handle)

// Since EPIC_6_3
CREATE_PACKET(TS_CS_MONSTER_RECOGNIZE, 517);

#endif // PACKETS_TS_CS_MONSTER_RECOGNIZE_H
