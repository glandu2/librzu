#include "ClientAuthSession.h"
#include "Cipher/AesPasswordCipher.h"
#include "Cipher/DesPasswordCipher.h"
#include "ClientGameSession.h"
#include "Packet/PacketEpics.h"
#include <memory>

#include "AuthClient/Flat/TS_AC_AES_KEY_IV.h"
#include "AuthClient/Flat/TS_AC_RESULT.h"
#include "AuthClient/Flat/TS_AC_RESULT_WITH_STRING.h"
#include "AuthClient/Flat/TS_AC_SELECT_SERVER.h"
#include "AuthClient/TS_AC_SERVER_LIST.h"
#include "GameClient/TS_SC_RESULT.h"

#include "AuthClient/Flat/TS_CA_ACCOUNT.h"
#include "AuthClient/Flat/TS_CA_RSA_PUBLIC_KEY.h"
#include "AuthClient/Flat/TS_CA_SELECT_SERVER.h"
#include "AuthClient/Flat/TS_CA_SERVER_LIST.h"
#include "AuthClient/Flat/TS_CA_VERSION.h"

DesPasswordCipher ClientAuthSession::desCipher("MERONG");

ClientAuthSession::ClientAuthSession(ClientGameSession* gameSession, int packetVersion)
    : gameSession(gameSession), packetVersion(packetVersion) {
	if(gameSession)
		gameSession->setAuthSession(this);
}

ClientAuthSession::~ClientAuthSession() {}

bool ClientAuthSession::connect(const std::string& ip,
                                uint16_t port,
                                const std::string& account,
                                const std::string& password,
                                AuthCipherMethod method,
                                const std::string& version) {
	this->username = account;
	this->password = password;
	this->version = version;
	this->cipherMethod = method;
	//	this->rsaCipher = 0;
	this->selectedServer = 0;
	this->normalDisconnect = false;

	return connect(ip.c_str(), port);
}

void ClientAuthSession::close() {
	closeSession();
	if(gameSession)
		gameSession->close();
}

void ClientAuthSession::retreiveServerList() {
	TS_CA_SERVER_LIST getServerListMsg;

	TS_MESSAGE::initMessage<TS_CA_SERVER_LIST>(&getServerListMsg);
	sendPacket(&getServerListMsg);
}

bool ClientAuthSession::selectServer(uint16_t serverId) {
	TS_CA_SELECT_SERVER selectServerMsg;

	selectedServer = -1;
	for(size_t i = 0; i < serverList.size(); i++) {
		if(serverList.at(i).id == serverId) {
			selectedServer = (int) i;
			break;
		}
	}
	if(selectedServer == -1) {
		log(LL_Error, "Can\'t select server, server id is invalid or server list must be retrieved before\n");
		return false;
	}

	TS_MESSAGE::initMessage<TS_CA_SELECT_SERVER>(&selectServerMsg);
	selectServerMsg.server_idx = serverId;
	sendPacket(&selectServerMsg);

	return true;
}

EventChain<SocketSession> ClientAuthSession::onDisconnected(bool causedByRemote) {
	if(normalDisconnect == false) {
		onAuthDisconnected();
	} else {
		normalDisconnect = false;
	}

	return PacketSession::onDisconnected(causedByRemote);
}

EventChain<PacketSession> ClientAuthSession::onPacketReceived(const TS_MESSAGE* packet) {
	switch(packet->id) {
		case TS_AC_AES_KEY_IV::packetID:
			onPacketAuthPasswordKey(reinterpret_cast<const TS_AC_AES_KEY_IV*>(packet));
			break;

		case TS_AC_RESULT::packetID: {
			const TS_AC_RESULT* resultMsg = reinterpret_cast<const TS_AC_RESULT*>(packet);
			if(resultMsg->request_msg_id == TS_CA_ACCOUNT::packetID) {
				onAuthResult((TS_ResultCode) resultMsg->result, std::string());
			}
			break;
		}

		case TS_AC_RESULT_WITH_STRING::packetID: {
			const TS_AC_RESULT_WITH_STRING* resultMsg = reinterpret_cast<const TS_AC_RESULT_WITH_STRING*>(packet);
			if(resultMsg->request_msg_id == TS_CA_ACCOUNT::packetID) {
				onAuthResult((TS_ResultCode) resultMsg->result, std::string(resultMsg->string, resultMsg->strSize));
			}
			break;
		}

		case TS_AC_SERVER_LIST::packetID: {
			packet->process(this, &ClientAuthSession::onPacketServerList, packetVersion);
			break;
		}

		case TS_AC_SELECT_SERVER::packetID:
			onPacketSelectServerResult(reinterpret_cast<const TS_AC_SELECT_SERVER*>(packet));
			break;
	}

	return PacketSession::onPacketReceived(packet);
}

////////////////////////////////////////////////////////

RsaCipher ClientAuthSession::rsaCipher;
EventChain<SocketSession> ClientAuthSession::onConnected() {
	TS_CA_VERSION versionMsg;
	memset(&versionMsg, 0, sizeof(versionMsg));

	TS_MESSAGE::initMessage<TS_CA_VERSION>(&versionMsg);

	strcpy(versionMsg.szVersion, version.c_str());
	sendPacket(&versionMsg);

	if(this->cipherMethod == ACM_DES) {
		TS_CA_ACCOUNT accountMsg;
		memset(&accountMsg, 0, sizeof(accountMsg));

		TS_MESSAGE::initMessage<TS_CA_ACCOUNT>(&accountMsg);

		static unsigned char cachedPassword[sizeof(accountMsg.password)] = {0};
		static std::string cachedPasswordStr;

		if(cachedPasswordStr != password) {
			strcpy(reinterpret_cast<char*>(cachedPassword), password.c_str());
			desCipher.encrypt(cachedPassword, sizeof(cachedPassword));
			cachedPasswordStr = password;
		}

		strcpy(accountMsg.account, username.c_str());
		memcpy(accountMsg.password, cachedPassword, sizeof(accountMsg.password));

		sendPacket(&accountMsg);
	} else if(this->cipherMethod == ACM_RSA_AES) {
		if(!rsaCipher.isInitialized()) {
			if(!rsaCipher.generateKey()) {
				log(LL_Error, "Failed to generate RSA key\n");
			}
		}

		if(rsaCipher.isInitialized()) {
			TS_CA_RSA_PUBLIC_KEY* keyMsg;
			std::vector<uint8_t> key;

			rsaCipher.getPemPublicKey(key);

			keyMsg = TS_MESSAGE_WNA::create<TS_CA_RSA_PUBLIC_KEY, unsigned char>(key.size());

			keyMsg->key_size = key.size();
			memcpy(keyMsg->key, key.data(), key.size());

			sendPacket(keyMsg);
			TS_MESSAGE_WNA::destroy(keyMsg);
		} else {
			log(LL_Error, "No RSA key to send, aborting connection\n");
			abortSession();
		}
	}

	return PacketSession::onConnected();
}

void ClientAuthSession::onPacketAuthPasswordKey(const TS_AC_AES_KEY_IV* packet) {
	TS_CA_ACCOUNT_RSA accountMsg;
	std::vector<uint8_t> encryptedPassword;

	TS_MESSAGE::initMessage<TS_CA_ACCOUNT_RSA>(&accountMsg);
	memset(accountMsg.account, 0, sizeof(accountMsg.account));

	if(!rsaCipher.privateDecrypt(packet->rsa_encrypted_data, packet->data_size, aesKey) || aesKey.size() != 32) {
		log(LL_Warning, "onPacketAuthPasswordKey: invalid decrypted data size: %d\n", (int) aesKey.size());
		closeSession();
		return;
	}

	AesPasswordCipher aesCipher;
	aesCipher.init(aesKey.data());

	if(!aesCipher.encrypt((const uint8_t*) password.c_str(), password.size(), encryptedPassword)) {
		log(LL_Warning, "onPacketAuthPasswordKey: could not encrypt password !\n");
		closeSession();
		return;
	}

	memcpy(accountMsg.password, encryptedPassword.data(), encryptedPassword.size());

	// password.fill(0);
	password.clear();

	strcpy(accountMsg.account, username.c_str());
	accountMsg.password_size = encryptedPassword.size();
	accountMsg.dummy[0] = accountMsg.dummy[1] = accountMsg.dummy[2] = 0;
	accountMsg.unknown_00000100 = 0x00000100;
	sendPacket(&accountMsg);
}

void ClientAuthSession::onPacketServerList(const TS_AC_SERVER_LIST* packet) {
	std::vector<ServerInfo> serverList;
	ServerInfo currentServerInfo;
	ServerConnectionInfo serverConnectionInfo;
	auto& packetServerList = packet->servers;

	this->serverList.clear();
	serverList.reserve(packetServerList.size());
	this->serverList.reserve(packetServerList.size());

	selectedServer = 0;

	for(size_t i = 0; i < packetServerList.size(); ++i) {
		currentServerInfo.serverId = packetServerList[i].server_idx;
		currentServerInfo.serverIp = packetServerList[i].server_ip;
		currentServerInfo.serverPort = packetServerList[i].server_port;
		currentServerInfo.serverName = packetServerList[i].server_name;
		currentServerInfo.serverScreenshotUrl = packetServerList[i].server_screenshot_url;
		currentServerInfo.userRatio = packetServerList[i].user_ratio;
		serverList.push_back(currentServerInfo);

		// Keep in memory server's ip & port for the server move
		serverConnectionInfo.id = currentServerInfo.serverId;
		serverConnectionInfo.ip = currentServerInfo.serverIp;
		serverConnectionInfo.port = currentServerInfo.serverPort;
		this->serverList.push_back(serverConnectionInfo);

		if(packetServerList[i].server_idx == packet->last_login_server_idx)
			selectedServer = (int) i;
	}

	onServerList(serverList, packet->last_login_server_idx);
}

void ClientAuthSession::onPacketSelectServerResult(const TS_AC_SELECT_SERVER* packet) {
	// int pendingTimeBeforeServerMove;

	if(this->cipherMethod == ACM_DES) {
		oneTimePassword = packet->one_time_key;
		// pendingTimeBeforeServerMove = packet->v1.pending_time;
	} else if(this->cipherMethod == ACM_RSA_AES) {
		const TS_AC_SELECT_SERVER_RSA* packetv2 = reinterpret_cast<const TS_AC_SELECT_SERVER_RSA*>(packet);
		std::vector<uint8_t> decryptedData;

		AesPasswordCipher aesCipher;
		aesCipher.init(aesKey.data());

		if(!aesCipher.decrypt(packetv2->encrypted_data, sizeof(packetv2->encrypted_data), decryptedData) ||
		   decryptedData.size() < sizeof(uint64_t)) {
			log(LL_Warning, "onPacketSelectServerResult: Could not decrypt TS_AC_SELECT_SERVER\n");
			closeSession();
			return;
		}

		oneTimePassword = *reinterpret_cast<uint64_t*>(decryptedData.data());
		// pendingTimeBeforeServerMove = packetv2->pending_time;
	}

	const ServerConnectionInfo& selectedServerInfo = serverList.at(selectedServer);
	if(gameSession)
		gameSession->connect(selectedServerInfo.ip.c_str(), selectedServerInfo.port);
	normalDisconnect = true;
	closeSession();

	std::vector<ServerConnectionInfo> empty;
	serverList.swap(empty);
}
