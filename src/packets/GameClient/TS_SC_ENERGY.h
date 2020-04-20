#ifndef PACKETS_TS_SC_ENERGY_H
#define PACKETS_TS_SC_ENERGY_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_ENERGY_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int16_t, energy)

CREATE_PACKET(TS_SC_ENERGY, 515);
#undef TS_SC_ENERGY_DEF

#endif // PACKETS_TS_SC_ENERGY_H
