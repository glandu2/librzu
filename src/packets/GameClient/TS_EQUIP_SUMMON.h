#ifndef PACKETS_TS_EQUIP_SUMMON_H
#define PACKETS_TS_EQUIP_SUMMON_H

#include "Packet/PacketDeclaration.h"

#define TS_EQUIP_SUMMON_DEF(_) \
	_(simple) (bool, open_dialog) \
	_(array) (ar_handle_t, card_handle, 6)

CREATE_PACKET(TS_EQUIP_SUMMON, 303);

#endif // PACKETS_TS_EQUIP_SUMMON_H
