#ifndef PACKETS_TS_AC_SERVER_LIST_H
#define PACKETS_TS_AC_SERVER_LIST_H

#include "PacketBaseMessage.h"

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


struct TS_SERVER_INFO_EX {
		uint16_t server_idx;
		char server_name[21];
		bool is_adult_server;
		char server_screenshot_url[256];
		char server_ip[16];
		int32_t server_port;
		uint16_t user_ratio;

		uint32_t getSize(int version) const {
				int size = 0;
				size += 2;
				size += 1 * 21;
				if(version >= 040000) {
						size += 1;
				}
				if(version >= 040000) {
						size += 1 * 256;
				}
				size += 1 * 16;
				size += 4;
				size += 2;

				return size;
		}

		template<class T>
		void serialize(T* buffer) const {
				buffer->write("server_idx", server_idx);
				buffer->write("server_name", server_name, 21);
				if(buffer->version >= 040000)
						buffer->write("is_adult_server", is_adult_server);
				if(buffer->version >= 040000)
						buffer->write("server_screenshot_url", server_screenshot_url, 256);
				buffer->write("server_ip", server_ip, 16);
				buffer->write("server_port", server_port);
				buffer->write("user_ratio", user_ratio);
		}

		template<class T>
		void deserialize(T* buffer) {
				buffer->read("server_idx", server_idx);
				buffer->read("server_name", server_name, 21);
				if(buffer->version >= 040000)
						buffer->read("is_adult_server", is_adult_server);
				else
						is_adult_server = false;
				if(buffer->version >= 040000)
						buffer->read("server_screenshot_url", server_screenshot_url, 256);
				else
						strcpy(server_screenshot_url, "about:blank");
				buffer->read("server_ip", server_ip, 16);
				buffer->read("server_port", server_port);
				buffer->read("user_ratio", user_ratio);
		}
};

struct TS_AC_SERVER_LIST_EX : public TS_MESSAGE_BASE {
		static const uint16_t packetId = 10022;
		TS_AC_SERVER_LIST_EX() : TS_MESSAGE_BASE(packetId) {}

		uint16_t last_login_server_idx;
		std::vector<TS_SERVER_INFO_EX> servers;
		uint32_t adata;

		uint32_t getSize(int version) const {
				int size = 0;
				if(version >= 040000) {
						size += 2;
				}
				size += 2;
				for(size_t i = 0; i < servers.size(); i++)
						size += servers[i].getSize(version);
				size += 4;

				return size;
		}

		template<class T>
		void serialize(T* buffer) const {
				TS_MESSAGE_BASE::serialize(buffer);

				if(buffer->version >= 040000)
						buffer->write("last_login_server_idx", last_login_server_idx);
				buffer->write("servers.size()", (uint16_t)servers.size());
				buffer->write("servers", servers);
				buffer->write("adata", adata);
		}

		template<class T>
		void deserialize(T* buffer) {
				TS_MESSAGE_BASE::deserialize(buffer);

				if(buffer->version >= 040000)
						buffer->read("last_login_server_idx", last_login_server_idx);
				else
						last_login_server_idx = 1;
				servers.resize(buffer->template read<uint16_t>("servers.size()"));
				buffer->read("servers", servers);
				buffer->read("adata", adata);
		}
};

#endif // PACKETS_TS_AC_SERVER_LIST_H
