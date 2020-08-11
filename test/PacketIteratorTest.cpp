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
	void operator()() {
		Object::logStatic(Object::LL_Debug,
		                  "FunctorCheckLatestId",
		                  "Checking %s: %d\n",
		                  Packet::getName(),
		                  Packet::getId(EPIC_LATEST));
		std::vector<SessionType> sessionTypes;
		std::vector<SessionPacketOrigin> origins;

		if(Packet::SESSION_TYPE != SessionType::Any) {
			sessionTypes.push_back(Packet::SESSION_TYPE);
		} else {
			sessionTypes.push_back(SessionType::AuthClient);
			sessionTypes.push_back(SessionType::GameClient);
		}

		if(Packet::ORIGIN != SessionPacketOrigin::Any) {
			origins.push_back(Packet::ORIGIN);
		} else {
			origins.push_back(SessionPacketOrigin::Client);
			origins.push_back(SessionPacketOrigin::Server);
		}

		for(size_t i = 0; i < sessionTypes.size(); i++) {
			for(size_t j = 0; j < origins.size(); j++) {
				int id = 0;
				processPacket<FunctorReturnLatestId>(
				    sessionTypes[i], origins[j], EPIC_LATEST, Packet::getId(EPIC_LATEST), id);
				EXPECT_EQ(id, Packet::getId(EPIC_LATEST))
				    << "with session type " << (int) sessionTypes[i] << " and origin " << (int) origins[j];
			}
		}
	}
};

TEST(PacketIterator, process) {
	iteratePackets<FunctorCheckLatestId>();
}
