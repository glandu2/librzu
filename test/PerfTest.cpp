#include "AuthClient/TS_AC_SERVER_LIST.h"
#include "Core/Utils.h"
#include "Packet/MessageBuffer.h"
#include "gtest/gtest.h"

TEST(PerfTest, PacketSerialization) {
	TS_AC_SERVER_LIST packet;
	TS_SERVER_INFO serverInfo[2];

	packet.last_login_server_idx = 5;

	serverInfo[0].server_idx = 1;
	serverInfo[0].server_name = "server 1";
	serverInfo[0].is_adult_server = false;
	serverInfo[0].server_screenshot_url = "http://screenshot.com/server1";
	serverInfo[0].server_ip = "127.0.0.1";
	serverInfo[0].server_port = 4514;
	serverInfo[0].user_ratio = 30;

	serverInfo[1].server_idx = 2;
	serverInfo[1].server_name = "server 2";
	serverInfo[1].is_adult_server = true;
	serverInfo[1].server_screenshot_url = "http://screenshot.com/server2";
	serverInfo[1].server_ip = "192.168.89.24";
	serverInfo[1].server_port = 4515;
	serverInfo[1].user_ratio = 64;

	packet.servers.push_back(serverInfo[0]);
	packet.servers.push_back(serverInfo[1]);

	uint64_t beginTime = uv_hrtime();
	int i;
	int version = EPIC_4_1;
	for(i = 0; i < 10000; i++) {
		MessageBuffer buffer(packet.getSize(version), version);
		packet.serialize(&buffer);
	}

	uint64_t duration = uv_hrtime() - beginTime;
	Object::logStatic(Object::LL_Info, "TS_AC_SERVER_LIST", "Serialization duration: %f ns\n", (double) duration / i);
}

TEST(PerfTest, getTimeInMsec) {
	int i;
	uint64_t beginTime;
	uint64_t duration;

	Utils::getTimeInMsec();

	beginTime = uv_hrtime();
	for(i = 0; i < 100000000; i++) {
		Utils::getTimeInMsec();
	}

	duration = uv_hrtime() - beginTime;
	Object::logStatic(Object::LL_Info, "Utils::getTimeInMsec", "Test duration: %f ns\n", (double) duration / i);
}
