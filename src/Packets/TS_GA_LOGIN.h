#ifndef PACKETS_TS_GA_LOGIN_H
#define PACKETS_TS_GA_LOGIN_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_GA_LOGIN : public TS_MESSAGE
{
	uint16_t server_idx;
	char server_name[21];
	char server_screenshot_url[256];
	bool is_adult_server;
	char server_ip[16];
	int32_t server_port;

	static const uint16_t packetID = 20001;
};
#pragma pack(pop)

#endif // PACKETS_TS_GA_LOGIN_H
