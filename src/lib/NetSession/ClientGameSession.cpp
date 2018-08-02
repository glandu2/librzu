#include "ClientGameSession.h"
#include "Cipher/RzHashReversible256.h"
#include "ClientAuthSession.h"
#include "GameClient/TS_CS_ACCOUNT_WITH_AUTH.h"
#include "GameClient/TS_CS_REPORT.h"
#include "GameClient/TS_CS_VERSION.h"
#include "GameClient/TS_SC_RESULT.h"

ClientGameSession::ClientGameSession(int version) : version(version) {}

EventChain<SocketSession> ClientGameSession::onConnected() {
	TS_CS_VERSION versionMsg;

	// continue server move as we are connected now to game server
	versionMsg.szVersion = auth->getGameVersion();
	RzHashReversible256::generatePayload(versionMsg);

	sendPacket(versionMsg);

	TS_CS_REPORT reportMsg;
	reportMsg.report =
	    "\x8D\x07\x72\xCA\x29\x47Windows (6.2.9200)|Intel(R) HD Graphics 4000Drv Version : 10.18.10.4425";
	sendPacket(reportMsg);

	TS_CS_ACCOUNT_WITH_AUTH loginInGameServerMsg;

	loginInGameServerMsg.account = auth->getAccountName();
	loginInGameServerMsg.one_time_key = auth->getOnTimePassword();
	sendPacket(loginInGameServerMsg);

	return PacketSession::onConnected();
}

EventChain<SocketSession> ClientGameSession::onDisconnected(bool causedByRemote) {
	onGameDisconnected();
	auth->onGameDisconnected();

	return PacketSession::onDisconnected(causedByRemote);
}

EventChain<PacketSession> ClientGameSession::onPacketReceived(const TS_MESSAGE* packet) {
	switch(packet->id) {
		case TS_SC_RESULT::packetID: {
			TS_SC_RESULT resultMsg;
			bool deserializationOk = packet->process(resultMsg, version);

			if(deserializationOk && resultMsg.request_msg_id == TS_CS_ACCOUNT_WITH_AUTH::packetID) {
				auth->onGameResult((TS_ResultCode) resultMsg.result);
				if(resultMsg.result == TS_RESULT_SUCCESS)
					onGameConnected();
			} else {
				onGamePacketReceived(packet);
			}
			break;
		}

		default:
			onGamePacketReceived(packet);
	}

	return PacketSession::onPacketReceived(packet);
}
