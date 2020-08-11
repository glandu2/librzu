#include "AutoClientSession.h"
#include "Cipher/RzHashReversible256.h"
#include "NetSession/ClientAuthSession.h"
#include <time.h>

#include "GameClient/TS_CS_CHARACTER_LIST.h"
#include "GameClient/TS_CS_GAME_TIME.h"
#include "GameClient/TS_CS_LOGIN.h"
#include "GameClient/TS_CS_LOGOUT.h"
#include "GameClient/TS_CS_UPDATE.h"
#include "GameClient/TS_SC_CHARACTER_LIST.h"
#include "GameClient/TS_SC_DISCONNECT_DESC.h"
#include "GameClient/TS_SC_GAME_TIME.h"
#include "GameClient/TS_SC_LOGIN_RESULT.h"
#include "GameClient/TS_SC_SET_TIME.h"
#include "GameClient/TS_TIMESYNC.h"

AutoAuthSession::AutoAuthSession(AutoClientSession* gameSession,
                                 const std::string& ip,
                                 uint16_t port,
                                 const std::string& account,
                                 const std::string& password,
                                 int serverIdx,
                                 int delayTime,
                                 int ggRecoTime,
                                 int epic)
    : ClientAuthSession(gameSession, epic),
      gameSession(gameSession),
      ip(ip),
      port(port),
      account(account),
      password(password),
      serverIdx(serverIdx),
      delayTime(delayTime),
      ggRecoTime(ggRecoTime) {}

void AutoAuthSession::connect() {
	ClientAuthSession::connect(ip, port, account, password);
}

void AutoAuthSession::delayedConnect() {
	if(delayTime > 0) {
		log(LL_Info, "Will connect to auth in %dms\n", delayTime);
		delayRecoTimer.start(this, &AutoAuthSession::onDelayRecoExpired, delayTime, 0);
	} else {
		connect();
	}
}

void AutoAuthSession::onDelayRecoExpired() {
	log(LL_Info, "End of delay connect timer, connecting to auth now\n");
	connect();
}

void AutoAuthSession::onAuthDisconnected(bool causedByRemote) {
	if(gameSession->onAuthDisconnected(causedByRemote))
		delayedConnect();
}

void AutoAuthSession::onAuthResult(TS_ResultCode result, const std::string& resultString) {
	if(result != TS_RESULT_SUCCESS) {
		log(LL_Error,
		    "%s: Auth failed result: %d (%s)\n",
		    account.c_str(),
		    result,
		    resultString.empty() ? "no associated string" : resultString.c_str());
		abortSession();
	} else {
		log(LL_Info, "Retrieving server list\n");
		retreiveServerList();
	}
}

void AutoAuthSession::onServerList(const std::vector<ServerInfo>& servers, uint16_t lastSelectedServerId) {
	bool serverFound = false;

	log(LL_Debug, "Server list (last id: %d)\n", lastSelectedServerId);
	for(size_t i = 0; i < servers.size(); i++) {
		const ServerInfo& serverInfo = servers.at(i);
		log(LL_Debug,
		    "%d: %20s at %16s:%d %d%% user ratio\n",
		    serverInfo.serverId,
		    serverInfo.serverName.c_str(),
		    serverInfo.serverIp.c_str(),
		    serverInfo.serverPort,
		    serverInfo.userRatio);

		if(serverInfo.serverId == serverIdx && !serverFound) {
			serverFound = true;
		}
	}

	if(!serverFound) {
		log(LL_Error, "Server with index %d not found\n", serverIdx);
		log(LL_Info, "Server list (last id: %d)\n", lastSelectedServerId);
		for(size_t i = 0; i < servers.size(); i++) {
			const ServerInfo& serverInfo = servers.at(i);
			log(LL_Info,
			    "%d: %20s at %16s:%d %d%% user ratio\n",
			    serverInfo.serverId,
			    serverInfo.serverName.c_str(),
			    serverInfo.serverIp.c_str(),
			    serverInfo.serverPort,
			    serverInfo.userRatio);
		}

		closeSession();
	} else {
		log(LL_Info, "Connecting to GS with index %d\n", serverIdx);
		selectServer(serverIdx);

		if(ggRecoTime > 0) {
			log(LL_Debug, "Starting GG timer: %ds\n", ggRecoTime);
			ggRecoTimer.start(this, &AutoAuthSession::onGGTimerExpired, ggRecoTime * 1000, 0);
		}
	}
}

void AutoAuthSession::onGameDisconnected(bool causedByRemote) {}

void AutoAuthSession::onGGTimerExpired() {
	log(LL_Info, "GG timeout, reconnecting\n");
	TS_CS_LOGOUT logoutPacket;
	gameSession->sendPacket(logoutPacket);
	gameSession->closeSession();
}

void AutoAuthSession::onGameResult(TS_ResultCode result) {
	if(result != TS_RESULT_SUCCESS) {
		log(LL_Error, "GS returned an error while authenticating: %d\n", result);
		gameSession->abortSession();
	} else {
		log(LL_Info, "Connected to GS\n");
	}
}

AutoClientSession::AutoClientSession(const std::string& ip,
                                     uint16_t port,
                                     const std::string& account,
                                     const std::string& password,
                                     int serverIdx,
                                     const std::string& playername,
                                     int epic,
                                     bool manageTimesync,
                                     int delayTime,
                                     int ggRecoTime)
    : ClientGameSession(epic),
      authSession(this, ip, port, account, password, serverIdx, delayTime, ggRecoTime, epic),
      manageTimesync(manageTimesync),
      playername(playername),
      connectedInGame(false),
      handle(0),
      gameTimeOffset{},
      epochTimeOffset(0) {}

void AutoClientSession::connect() {
	authSession.setPacketLogger(getPacketLogger());
	authSession.connect();
}

void AutoClientSession::setConnected(bool connected) {
	if(connected && !connectedInGame) {
		if(manageTimesync) {
			TS_CS_GAME_TIME gameTimePkt;
			sendPacket(gameTimePkt);

			updateTimer.start(this, &AutoClientSession::onUpdatePacketExpired, 5000, 5000);
		}

		onConnected();
	} else if(!connected && connectedInGame) {
		onDisconnected();
		updateTimer.stop();
	}
	connectedInGame = connected;
}

void AutoClientSession::onGameConnected() {
	TS_CS_CHARACTER_LIST charlistPkt;
	charlistPkt.account = auth->getAccountName();
	sendPacket(charlistPkt);
}

void AutoClientSession::onGameDisconnected(bool causedByRemote) {
	setConnected(false);
	authSession.delayedConnect();
}

void AutoClientSession::onUpdatePacketExpired() {
	if(!connectedInGame)
		return;

	TS_CS_UPDATE updatPkt;

	updatPkt.handle = handle;
	updatPkt.time = getGameTime();
	updatPkt.epoch_time = static_cast<uint32_t>(getEpochTime());

	sendPacket(updatPkt);
}

ar_time_t AutoClientSession::getLocalGameTime() {
	return ar_time_t(uint32_t(uv_hrtime() / (10 * 1000 * 1000)));
}

void AutoClientSession::onGamePacketReceived(const TS_MESSAGE* packet) {
	packet_type_id_t packetType = PacketMetadata::convertPacketIdToTypeId(
	    packet->id, SessionType::GameClient, SessionPacketOrigin::Server, packetVersion);
	switch(packetType) {
		case TS_SC_CHARACTER_LIST::packetID:
			packet->process(this, &AutoClientSession::onCharacterList, packetVersion);
			break;

		case TS_SC_LOGIN_RESULT::packetID:
			packet->process(this, &AutoClientSession::onCharacterLoginResult, packetVersion);
			break;

		case TS_SC_DISCONNECT_DESC::packetID:
			setConnected(false);
			abortSession();
			break;

		case TS_TIMESYNC::packetID:
			packet->process(this, &AutoClientSession::onTimeSync, packetVersion);
			break;

		case TS_SC_SET_TIME::packetID:
			packet->process(this, &AutoClientSession::onSetTime, packetVersion);
			break;

		case TS_SC_GAME_TIME::packetID:
			packet->process(this, &AutoClientSession::onGameTime, packetVersion);
			break;
	}

	// TIMESYNC packet is sent by the server just before TS_SC_LOGIN_RESULT
	if(isConnected() || packet->id == TS_TIMESYNC::getId(packetVersion))
		onPacketReceived(packet);
}

void AutoClientSession::onCharacterList(const TS_SC_CHARACTER_LIST* packet) {
	bool characterInList = false;

	log(LL_Debug, "Character list: \n");
	for(size_t i = 0; i < packet->characters.size(); i++) {
		log(LL_Debug, " - %s\n", packet->characters[i].name.c_str());
		if(playername == packet->characters[i].name)
			characterInList = true;
	}

	if(!characterInList) {
		log(LL_Warning, "Character \"%s\" not in character list: \n", playername.c_str());
		for(size_t i = 0; i < packet->characters.size(); i++) {
			log(LL_Warning, " - %s\n", packet->characters[i].name.c_str());
		}
	}

	TS_CS_LOGIN loginPkt;
	loginPkt.name = playername;
	loginPkt.race = 0;
	RzHashReversible256::generatePayload(loginPkt);
	sendPacket(loginPkt);
}

void AutoClientSession::onCharacterLoginResult(const TS_SC_LOGIN_RESULT* packet) {
	handle = packet->handle;
	log(LL_Info, "Connected with character %s\n", playername.c_str());
	setConnected(true);
}

void AutoClientSession::onTimeSync(const TS_TIMESYNC* packet) {
	gameTimeOffset = packet->time - getLocalGameTime();

	if(manageTimesync) {
		TS_TIMESYNC timeSyncPkt;
		timeSyncPkt.time = packet->time;
		sendPacket(timeSyncPkt);
	}
}

void AutoClientSession::onSetTime(const TS_SC_SET_TIME* packet) {
	gameTimeOffset = packet->gap;
}

void AutoClientSession::onGameTime(const TS_SC_GAME_TIME* packet) {
	gameTimeOffset = packet->t.get() - getLocalGameTime().get();
	epochTimeOffset = int32_t(packet->game_time - time(nullptr));
}
