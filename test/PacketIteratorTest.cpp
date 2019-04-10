#include "PacketIterator.h"
#include "gtest/gtest.h"
#include <stdio.h>

template<class Packet> struct FunctorPrint {
	void operator()() {
		Object::logStatic(
		    Object::LL_Debug, "FunctorReturnLatestId", "%s: %d\n", Packet::getName(), Packet::getId(EPIC_LATEST));
	}
};

TEST(PacketIterator, iterator) {
	iteratePackets<FunctorPrint>();
}

template<class Packet> struct FunctorReturnLatestId {
	int operator()() {
		Object::logStatic(Object::LL_Debug,
		                  "FunctorReturnLatestId",
		                  "   Got %s: %d\n",
		                  Packet::getName(),
		                  Packet::getId(EPIC_LATEST));
		return Packet::getId(EPIC_LATEST);
	}
};

template<class Packet> struct FunctorCheckLatestId {
	void operator()(const char* prefix) {
		int id[4] = {-1, -1, -1, -1};

		Object::logStatic(Object::LL_Debug,
		                  "FunctorCheckLatestId",
		                  "Checking %s  %s: %d\n",
		                  prefix,
		                  Packet::getName(),
		                  Packet::getId(EPIC_LATEST));
		processPacket<FunctorReturnLatestId>(
		    SessionType::AuthClient, SessionPacketOrigin::Client, Packet::getId(EPIC_LATEST), id[0]);
		processPacket<FunctorReturnLatestId>(
		    SessionType::AuthClient, SessionPacketOrigin::Server, Packet::getId(EPIC_LATEST), id[1]);
		processPacket<FunctorReturnLatestId>(
		    SessionType::GameClient, SessionPacketOrigin::Client, Packet::getId(EPIC_LATEST), id[2]);
		processPacket<FunctorReturnLatestId>(
		    SessionType::GameClient, SessionPacketOrigin::Server, Packet::getId(EPIC_LATEST), id[3]);
		EXPECT_TRUE(Packet::getId(EPIC_LATEST) == id[0] || Packet::getId(EPIC_LATEST) == id[1] ||
		            Packet::getId(EPIC_LATEST) == id[2] || Packet::getId(EPIC_LATEST) == id[3])
		    << "Expected one of " << id;
	}
};

TEST(PacketIterator, process) {
	iteratePackets<FunctorCheckLatestId>("Client -> Auth  ");
	iteratePackets<FunctorCheckLatestId>("Auth   -> Client");
	iteratePackets<FunctorCheckLatestId>("Client -> GS    ");
	iteratePackets<FunctorCheckLatestId>("GS     -> Client");
}
