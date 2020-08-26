#include "PacketStructsName.h"
#include "Core/Object.h"
#include "Core/Utils.h"
#include "PacketDeclaration.h"
#include "PacketEpics.h"
#include <initializer_list>
#include <stddef.h>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "PacketIterator.h"

struct PacketNameInfo {
	unsigned short id;
	const char* name;
};

struct PacketInfo {
	uint16_t (*getId)(packet_version_t version);
	packet_type_id_t id;
	SessionType sessionType;
	SessionPacketOrigin packetDir;
	const char* name;
};

static std::vector<std::vector<PacketInfo>> packetsInfo;

namespace PacketMetadata {

static void addPacketMetadataForId(uint16_t packetId,
                                   SessionType sessionType,
                                   SessionPacketOrigin packetDir,
                                   uint16_t (*getId)(packet_version_t version),
                                   packet_type_id_t id,
                                   const char* name) {
	if(packetsInfo.size() <= packetId)
		packetsInfo.resize(packetId + 1);
	packetsInfo[packetId].push_back({getId, id, sessionType, packetDir, name});
}
template<class Packet> struct FunctorAddPacketType {
	void operator()() {
		for(auto packetId : Packet::PACKET_IDS::getAsInitializerList()) {
			addPacketMetadataForId(
			    packetId, Packet::SESSION_TYPE, Packet::ORIGIN, &Packet::getId, Packet::packetID, Packet::getName());
		}
	}
};

static void initPacketsInfo() {
	iteratePackets<FunctorAddPacketType>();
}

const PacketInfo* getPacketMetadata(uint16_t id,
                                    SessionType sessionType,
                                    SessionPacketOrigin packetDir,
                                    packet_version_t version) {
	if(packetsInfo.empty())
		initPacketsInfo();
	if(packetsInfo.size() > id) {
		const auto& packetsForId = packetsInfo[id];
		for(const PacketInfo& packetInfo : packetsForId) {
			if(sessionType != SessionType::Any && packetInfo.sessionType != SessionType::Any &&
			   packetInfo.sessionType != sessionType)
				continue;
			if(packetDir != SessionPacketOrigin::Any && packetInfo.packetDir != SessionPacketOrigin::Any &&
			   packetInfo.packetDir != packetDir)
				continue;
			if(packetInfo.getId(version) == id)
				return &packetInfo;
		}
	}
	Object::logStatic(Object::LL_Error,
	                  "convertPacketIdToTypeId",
	                  "Unknown packet ID %u for session type %u, origin %u and version 0x%x\n",
	                  id,
	                  (uint32_t) sessionType,
	                  (uint32_t) packetDir,
	                  version.getAsInt());
	return nullptr;
}

packet_type_id_t convertPacketIdToTypeId(uint16_t id,
                                         SessionType sessionType,
                                         SessionPacketOrigin packetDir,
                                         packet_version_t version) {
	const PacketInfo* packetInfo = getPacketMetadata(id, sessionType, packetDir, version);
	if(packetInfo)
		return packetInfo->id;
	return static_cast<packet_type_id_t>(id);
}

const char* getPacketName(unsigned int id,
                          SessionType sessionType,
                          SessionPacketOrigin packetDir,
                          packet_version_t version) {
	const PacketInfo* packetInfo = getPacketMetadata(id, sessionType, packetDir, version);
	if(packetInfo)
		return packetInfo->name;

	return nullptr;
}

SessionPacketOrigin getPacketOriginFromDirection(bool outgoing, SessionPacketOrigin selfSessionPacketOrigin) {
	if(selfSessionPacketOrigin == SessionPacketOrigin::Any)
		return SessionPacketOrigin::Any;

	if(outgoing)
		return selfSessionPacketOrigin;

	// If ingoing, return opposite origin

	if(selfSessionPacketOrigin == SessionPacketOrigin::Client)
		return SessionPacketOrigin::Server;

	return SessionPacketOrigin::Client;
}

void getPacketOriginName(bool outgoing,
                         SessionType sessionType,
                         SessionPacketOrigin selfSessionPacketOrigin,
                         const char** fromNamePtr,
                         const char** toNamePtr) {
	const char* serverName = nullptr;
	const char* clientName = nullptr;

	switch(sessionType) {
		case SessionType::AuthClient:
			serverName = "Auth";
			clientName = "Client";
			break;
		case SessionType::AuthGame:
			serverName = "Auth";
			clientName = "Game";
			break;
		case SessionType::GameClient:
			serverName = "Game";
			clientName = "Client";
			break;
		case SessionType::UploadClient:
			serverName = "Upload";
			clientName = "Client";
			break;
		case SessionType::UploadGame:
			serverName = "Upload";
			clientName = "Game";
			break;
		default:
			break;
	}

	SessionPacketOrigin origin = getPacketOriginFromDirection(outgoing, selfSessionPacketOrigin);
	const char* fromName = nullptr;
	const char* toName = nullptr;

	if(origin == SessionPacketOrigin::Client) {
		fromName = clientName;
		toName = serverName;
	} else if(origin == SessionPacketOrigin::Server) {
		fromName = serverName;
		toName = clientName;
	}

	if(fromName == nullptr) {
		if(outgoing)
			fromName = "local";
		else
			fromName = "remote";
	}
	if(toName == nullptr) {
		if(outgoing)
			toName = "remote";
		else
			toName = "local";
	}

	if(fromNamePtr)
		*fromNamePtr = fromName;

	if(toNamePtr)
		*toNamePtr = toName;
}

}  // namespace PacketMetadata
