#ifndef PACKETS_TS_SC_AUCTION_SEARCH_H
#define PACKETS_TS_SC_AUCTION_SEARCH_H

#include "Packet/PacketDeclaration.h"
#include "TS_SC_INVENTORY.h"

#define TS_AUCTION_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (int32_t , auction_uid) \
	simple_ (TS_ITEM_BASE_INFO, item_info) \
	simple_ (uint8_t, duration_type) \
	simple_ (uint64_t, bidded_price) \
	simple_ (uint64_t, instant_purchase_price) \
	string_ (seller_name, 31) \
	simple_ (uint8_t, flag)
CREATE_STRUCT(TS_AUCTION_INFO);

#define TS_SC_AUCTION_SEARCH_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_(int32_t, page_num) \
	simple_(int32_t, total_page_count) \
	simple_(int32_t, auction_info_count) \
	array_ (TS_AUCTION_INFO, auction_info, 40)

CREATE_PACKET(TS_SC_AUCTION_SEARCH, 1301);

#endif // PACKETS_TS_SC_AUCTION_SEARCH_H
