#ifndef PACKETS_TS_SC_DIALOG_H
#define PACKETS_TS_SC_DIALOG_H

#include "Packet/PacketDeclaration.h"

// strings len does not include null terminator
// Server does not include null terminator in strings
#define TS_SC_DIALOG_DEF(_) \
	_(simple)(int32_t, type) \
	_(simple)(ar_handle_t, npc_handle) \
	_(count)(uint16_t, title) \
	_(count)(uint16_t, text) \
	_(count)(uint16_t, menu) \
	_(count)(uint16_t, script, version >= EPIC_9_6_2) \
	_(dynstring)(title, false) \
	_(dynstring)(text, false) \
	_(dynstring)(menu, false) \
	_(dynstring)(script, false, version >= EPIC_9_6_2)

CREATE_PACKET(TS_SC_DIALOG, 3000);
#undef TS_SC_DIALOG_DEF

#endif // PACKETS_TS_SC_DIALOG_H
