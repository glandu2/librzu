#pragma once

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AC_SERVER_LIST : public TS_MESSAGE_WNA
{
	uint16_t last_login_server_idx;
	uint16_t count;

	struct TS_SERVER_INFO
	{
		uint16_t server_idx;
		char server_name[21];
		bool is_adult_server;
		char server_screenshot_url[256];
		char server_ip[16];
		int32_t server_port;
		uint16_t user_ratio;
	} servers[0];

	static const uint16_t packetID = 10022;
};

struct TS_AC_SERVER_LIST_EPIC2 : public TS_MESSAGE_WNA
{
	uint16_t count;

	struct TS_SERVER_INFO
	{
		uint16_t server_idx;
		char server_name[21];
		char server_ip[16];
		int32_t server_port;
		uint16_t user_ratio;
	} servers[0];

	static const uint16_t packetID = 10022;
};
#pragma pack(pop)

