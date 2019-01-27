#include "AuthClient/TS_AC_SERVER_LIST.h"
#include "Core/Object.h"
#include "Packet/JSONWriter.h"
#include "gtest/gtest.h"

/* Tests:
 * TestPacketServer => Test, name
 * TestPacketSession => TestPacketServer
 * Test: onPacketReceived(TestPacketSession, std::string sourceName / enum, const Packet)
 * test dans dll
 * ProcessSpawner => config => name / enum (used to generate pipe name)
 */

TEST(JSONWriterTest, TS_AC_SERVER_LIST) {
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

	{
		Object::logStatic(Object::LL_Info, "gettimeofday", "Version 0 serialization\n");
		JSONWriter jsonWriter(0, false);
		packet.serialize(&jsonWriter);
		jsonWriter.finalize();
		ASSERT_GT(jsonWriter.toString().size(), 0) << "Version 0 serialization failed";
	}

	{
		Object::logStatic(Object::LL_Info, "gettimeofday", "Version 100000000 serialization\n");
		JSONWriter jsonWriter(100000000, false);
		packet.serialize(&jsonWriter);
		jsonWriter.finalize();
		ASSERT_GT(jsonWriter.toString().size(), 0) << "Version 100000000 serialization failed";
	}
}
