#ifndef PACKETS_TS_SC_EQUIP_SUMMON_H
#define PACKETS_TS_SC_EQUIP_SUMMON_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_EQUIP_SUMMON_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (bool, open_dialog) \
	simple_ (uint32_t, card_handle, 6)

CREATE_PACKET(TS_SC_EQUIP_SUMMON, 303);

#endif // PACKETS_TS_SC_EQUIP_SUMMON_H
