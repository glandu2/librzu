#include "ClientAuthSession.h"
#include "Packets/AuthPackets.h"
#include "DesPasswordCipher.h"
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include "ClientGameSession.h"
#include "Packets/Epics.h"

DesPasswordCipher ClientAuthSession::desCipher("MERONG");

ClientAuthSession::ClientAuthSession(ClientGameSession* gameSession)
	: gameSession(gameSession)
{
	if(gameSession)
		gameSession->setAuthSession(this);
}

ClientAuthSession::~ClientAuthSession() {
}

bool ClientAuthSession::connect(const std::string& ip, uint16_t port, const std::string& account, const std::string& password, AuthCipherMethod method, const std::string& version) {
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
	getStream()->close();
	if(gameSession)
		gameSession->getStream()->close();
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
			selectedServer = (int)i;
			break;
		}
	}
	if(selectedServer == -1) {
		error("Can\'t select server, server id is invalid or server list must be retrieved before\n");
		return false;
	}

	TS_MESSAGE::initMessage<TS_CA_SELECT_SERVER>(&selectServerMsg);
	selectServerMsg.server_idx = serverId;
	sendPacket(&selectServerMsg);

	return true;
}

void ClientAuthSession::onDisconnected(bool causedByRemote) {
	if(normalDisconnect == false) {
		onAuthDisconnected();
	} else {
		normalDisconnect = false;
	}
}

void ClientAuthSession::onPacketReceived(const TS_MESSAGE* packetData) {
	switch(packetData->id) {
		case TS_AC_AES_KEY_IV::packetID:
			onPacketAuthPasswordKey(reinterpret_cast<const TS_AC_AES_KEY_IV*>(packetData));
			break;

		case TS_AC_RESULT::packetID:
		{
			const TS_AC_RESULT* resultMsg = reinterpret_cast<const TS_AC_RESULT*>(packetData);
			if(resultMsg->request_msg_id == TS_CA_ACCOUNT::packetID) {
				onAuthResult((TS_ResultCode)resultMsg->result, std::string());
			}
			break;
		}

		case TS_AC_RESULT_WITH_STRING::packetID:
		{
			const TS_AC_RESULT_WITH_STRING* resultMsg = reinterpret_cast<const TS_AC_RESULT_WITH_STRING*>(packetData);
			if(resultMsg->request_msg_id == TS_CA_ACCOUNT::packetID) {
				onAuthResult((TS_ResultCode)resultMsg->result, std::string(resultMsg->string, resultMsg->strSize));
			}
			break;
		}

		case TS_AC_SERVER_LIST::packetID: {
			packetData->process(this, &ClientAuthSession::onPacketServerList, EPIC_9_1);
			break;
		}

		case TS_AC_SELECT_SERVER::packetID:
			onPacketSelectServerResult(reinterpret_cast<const TS_AC_SELECT_SERVER*>(packetData));
			break;
	}
}

////////////////////////////////////////////////////////

void* ClientAuthSession::rsaCipher = nullptr;
void ClientAuthSession::onConnected() {
	TS_CA_VERSION versionMsg;

	TS_MESSAGE::initMessage<TS_CA_VERSION>(&versionMsg);
#ifndef NDEBUG
	memset(versionMsg.szVersion, 0, sizeof(versionMsg.szVersion));
#endif
	strcpy(versionMsg.szVersion, version.c_str());
	sendPacket(&versionMsg);

	if(this->cipherMethod == ACM_DES) {
		TS_CA_ACCOUNT accountMsg;

		TS_MESSAGE::initMessage<TS_CA_ACCOUNT>(&accountMsg);

#ifndef NDEBUG
		memset(accountMsg.account, 0, sizeof(accountMsg.account));
		memset(accountMsg.password, 0, sizeof(accountMsg.password));
#endif
		strcpy(accountMsg.account, username.c_str());

		static unsigned char cachedPassword[sizeof(accountMsg.password)] = {0};
		static std::string cachedPasswordStr;

		if(cachedPasswordStr != password) {
			strcpy(reinterpret_cast<char*>(cachedPassword), password.c_str());
			desCipher.encrypt(cachedPassword, sizeof(cachedPassword));
			cachedPasswordStr = password;
		}

		memcpy(accountMsg.password, cachedPassword, sizeof(accountMsg.password));

		sendPacket(&accountMsg);
	} else if(this->cipherMethod == ACM_RSA_AES) {
		TS_CA_RSA_PUBLIC_KEY *keyMsg;
		int public_key_size;

		if(!rsaCipher)
			rsaCipher = RSA_generate_key(1024, 65537, NULL, NULL);

		BIO * b = BIO_new(BIO_s_mem());
		PEM_write_bio_RSA_PUBKEY(b, (RSA*)rsaCipher);

		public_key_size = BIO_get_mem_data(b, NULL);
		keyMsg = TS_MESSAGE_WNA::create<TS_CA_RSA_PUBLIC_KEY, unsigned char>(public_key_size);

		keyMsg->key_size = public_key_size;

		BIO_read(b, keyMsg->key, public_key_size);
		BIO_free(b);

		sendPacket(keyMsg);
		TS_MESSAGE_WNA::destroy(keyMsg);
	}
}

void ClientAuthSession::onPacketAuthPasswordKey(const TS_AC_AES_KEY_IV* packet) {
	TS_CA_ACCOUNT_RSA accountMsg;
	unsigned char decrypted_data[256];
	int data_size;
	bool ok = false;

	TS_MESSAGE::initMessage<TS_CA_ACCOUNT_RSA>(&accountMsg);
	memset(accountMsg.account, 0, sizeof(accountMsg.account));

	EVP_CIPHER_CTX e_ctx;
	const unsigned char *key_data = decrypted_data;
	const unsigned char *iv_data = decrypted_data + 16;
	int len = (int)password.size();
	int p_len = len, f_len = 0;

	data_size = RSA_private_decrypt(packet->data_size, (unsigned char*)packet->rsa_encrypted_data, decrypted_data, (RSA*)rsaCipher, RSA_PKCS1_PADDING);
//	RSA_free((RSA*)rsaCipher);
//	rsaCipher = 0;
	if(data_size != 32) {
		warn("onPacketAuthPasswordKey: invalid decrypted data size: %d\n", data_size);
		getStream()->close();
		return;
	}

	memcpy(aes_key_iv, decrypted_data, 32);
	memset(accountMsg.password, 0, sizeof(accountMsg.password));

	EVP_CIPHER_CTX_init(&e_ctx);

	if(!EVP_EncryptInit_ex(&e_ctx, EVP_aes_128_cbc(), NULL, key_data, iv_data))
		goto end;
	if(!EVP_EncryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL))
		goto end;
	if(!EVP_EncryptUpdate(&e_ctx, accountMsg.password, &p_len, (const unsigned char*)password.c_str(), len))
		goto end;
	if(!EVP_EncryptFinal_ex(&e_ctx, accountMsg.password+p_len, &f_len))
		goto end;

	ok = true;

end:
	EVP_CIPHER_CTX_cleanup(&e_ctx);

	if(ok == false) {
		warn("onPacketAuthPasswordKey: could not encrypt password !\n");
		getStream()->close();
		return;
	}

	//password.fill(0);
	password.clear();

	strcpy(accountMsg.account, username.c_str());
	accountMsg.password_size = p_len + f_len;
	accountMsg.dummy[0] = accountMsg.dummy[1] = accountMsg.dummy[2] = 0;
	accountMsg.unknown_00000100 = 0x00000100;
	sendPacket(&accountMsg);
}

void ClientAuthSession::onPacketServerList(const TS_AC_SERVER_LIST* packet) {
	std::vector<ServerInfo> serverList;
	ServerInfo currentServerInfo;
	ServerConnectionInfo serverConnectionInfo;
	const std::vector<TS_SERVER_INFO>& packetServerList = packet->servers;

	serverList.reserve(packetServerList.size());
	selectedServer = 0;
	this->serverList.clear();

	for(size_t i = 0; i < packetServerList.size(); ++i) {
		currentServerInfo.serverId = packetServerList[i].server_idx;
		currentServerInfo.serverIp = packetServerList[i].server_ip;
		currentServerInfo.serverPort = packetServerList[i].server_port;
		currentServerInfo.serverName = packetServerList[i].server_name;
		currentServerInfo.serverScreenshotUrl = packetServerList[i].server_screenshot_url;
		currentServerInfo.userRatio = packetServerList[i].user_ratio;
		serverList.push_back(currentServerInfo);

		//Keep in memory server's ip & port for the server move
		serverConnectionInfo.id = currentServerInfo.serverId;
		serverConnectionInfo.ip = currentServerInfo.serverIp;
		serverConnectionInfo.port = currentServerInfo.serverPort;
		this->serverList.push_back(serverConnectionInfo);

		if(packetServerList[i].server_idx == packet->last_login_server_idx)
			selectedServer = (int)i;
	}

	onServerList(serverList, packet->last_login_server_idx);
}

void ClientAuthSession::onPacketSelectServerResult(const TS_AC_SELECT_SERVER* packet) {
	//int pendingTimeBeforeServerMove;

	if(this->cipherMethod == ACM_DES) {
		oneTimePassword = packet->one_time_key;
		//pendingTimeBeforeServerMove = packet->v1.pending_time;
	} else if(this->cipherMethod == ACM_RSA_AES) {
		const TS_AC_SELECT_SERVER_RSA* packetv2 = reinterpret_cast<const TS_AC_SELECT_SERVER_RSA*>(packet);
		EVP_CIPHER_CTX e_ctx;
		const unsigned char *key_data = aes_key_iv;
		const unsigned char *iv_data = aes_key_iv + 16;
		int len = 16;
		int p_len = 0, f_len = 0;
		bool ok = false;
		uint64_t decryptedData[2];

		EVP_CIPHER_CTX_init(&e_ctx);
		if(!EVP_DecryptInit_ex(&e_ctx, EVP_aes_128_cbc(), NULL, key_data, iv_data))
			goto end;
		if(!EVP_DecryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL))
			goto end;
		if(!EVP_DecryptUpdate(&e_ctx, (unsigned char*)decryptedData, &p_len, packetv2->encrypted_data, len))
			goto end;
		if(!EVP_DecryptFinal_ex(&e_ctx, ((unsigned char*)decryptedData) + p_len, &f_len))
			goto end;
		ok = true;
end:
		EVP_CIPHER_CTX_cleanup(&e_ctx);

		if(ok == false) {
			warn("onPacketSelectServerResult: Could not decrypt TS_AC_SELECT_SERVER\n");
			getStream()->close();
			return;
		}

		oneTimePassword = decryptedData[0];
		//pendingTimeBeforeServerMove = packetv2->pending_time;
	}

	const ServerConnectionInfo& selectedServerInfo = serverList.at(selectedServer);
	if(gameSession)
		gameSession->connect(selectedServerInfo.ip.c_str(), selectedServerInfo.port);
	normalDisconnect = true;
	getStream()->close();

	std::vector<ServerConnectionInfo> empty;
	serverList.swap(empty);
}
