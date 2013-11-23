#include "Authentication.h"
#include "../Packets/AuthPackets.h"
#include "Server.h"
#include "DESPasswordCipher.h"
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#define LOG_PREFIX "Auth "

Authentication::Authentication(Server *server) : canBeDeleted(true) {
	this->server = server;
	this->rsaCipher = 0;
	this->aes_key_iv = 0;
	this->currentState = AS_Idle;

	server->setAuth(this);
	server->addPacketListener(Server::ST_Auth, TS_CC_EVENT::packetID, &onAuthServerConnectionEvent);
	server->addPacketListener(Server::ST_Auth, TS_AC_AES_KEY_IV::packetID, &onAuthPacketReceived);
	server->addPacketListener(Server::ST_Auth, TS_AC_RESULT::packetID, &onAuthPacketReceived);
	server->addPacketListener(Server::ST_Auth, TS_AC_RESULT_WITH_STRING::packetID, &onAuthPacketReceived);
	server->addPacketListener(Server::ST_Auth, TS_AC_SERVER_LIST::packetID, &onAuthPacketReceived);
	server->addPacketListener(Server::ST_Auth, TS_AC_SELECT_SERVER::packetID, &onAuthPacketReceived);

	server->addPacketListener(Server::ST_Game, TS_CC_EVENT::packetID, &onGameServerConnectionEvent);
	server->addPacketListener(Server::ST_Game, TS_CS_ACCOUNT_WITH_AUTH::packetID, &onGamePacketReceived);
}

Authentication::~Authentication() {
	if(!canBeDeleted)
		qWarning(LOG_PREFIX"Authentication object delete but authentication not terminated !");
	server->setAuth(0);
	if(aes_key_iv)
		delete[] aes_key_iv;
}

void Authentication::setServer(QByteArray host, quint16 port) {
	server->setServerFarm(host, port);
	if(server->getState() != Server::SS_NotConnected)
		server->close();
	server->connectToAuth();
}

void Authentication::connectAccount(QByteArray username, QByteArray password, QByteArray clientVersion, AuthCipherMethod cipherMethod) {
	this->clientVersion = clientVersion;
	this->username = username;
	this->password = password;
	this->cipherMethod = cipherMethod;
	password.fill(0);

	canBeDeleted = false;
	currentState = AS_ProcessLogin;
	if(server->getState() == Server::SS_NotConnected)
		server->connectToAuth();
	else if(server->getState() == Server::SS_ConnectedToAuth)
		onPacketAuthConnected();
}

void Authentication::abortConnection() {
	server->close();
	currentState = AS_Idle;
	canBeDeleted = true;
}

void Authentication::retreiveServerList() {
	TS_CA_SERVER_LIST getServerListMsg;

	if(currentState == AS_Idle)
		return;

	TS_MESSAGE::initMessage<TS_CA_SERVER_LIST>(&getServerListMsg);
	server->sendPacket(&getServerListMsg, Server::ST_Auth);
}

void Authentication::selectServer(quint16 serverId) {
	TS_CA_SELECT_SERVER selectServerMsg;

	currentState = AS_ProcessServerMove;

	for(int i = 0; i < serverList.size(); i++) {
		if(serverList.at(i).id == serverId) {
			selectedServer = i;
			break;
		}
	}

	TS_MESSAGE::initMessage<TS_CA_SELECT_SERVER>(&selectServerMsg);
	selectServerMsg.server_idx = serverId;
	server->sendPacket(&selectServerMsg, Server::ST_Auth);
}

void Authentication::onAuthServerConnectionEvent(Server* server, const TS_MESSAGE* packetData) {
	Authentication* thisAccount = server->getAuth();

	if(packetData->id == TS_CC_EVENT::packetID) {
		const TS_CC_EVENT* eventMsg = reinterpret_cast<const TS_CC_EVENT*>(packetData);

		if(eventMsg->event == TS_CC_EVENT::CE_ServerConnected)
			thisAccount->onPacketAuthConnected();
		else if(eventMsg->event != TS_CC_EVENT::CE_ServerDisconnected)
			thisAccount->onPacketAuthUnreachable();
	}
}

void Authentication::onGameServerConnectionEvent(Server* server, const TS_MESSAGE* packetData) {
	Authentication* thisAccount = server->getAuth();

	if(packetData->id == TS_CC_EVENT::packetID) {
		const TS_CC_EVENT* eventMsg = reinterpret_cast<const TS_CC_EVENT*>(packetData);

		if(eventMsg->event == TS_CC_EVENT::CE_ServerConnected)
			thisAccount->onPacketGameConnected();
		else
			thisAccount->onPacketGameUnreachable();
	}
}

void Authentication::onAuthPacketReceived(Server* server, const TS_MESSAGE* packetData) {
	Authentication* thisAccount = server->getAuth();

	switch(packetData->id) {
		case TS_AC_AES_KEY_IV::packetID:
			thisAccount->onPacketAuthPasswordKey(reinterpret_cast<const TS_AC_AES_KEY_IV*>(packetData));
			break;

		case TS_AC_RESULT::packetID:
		{
			const TS_AC_RESULT* resultMsg = reinterpret_cast<const TS_AC_RESULT*>(packetData);
			if(resultMsg->request_msg_id == TS_CA_ACCOUNT::packetID) {
				emit thisAccount->onLoginResult(resultMsg->result == 0);
			}
			break;
		}

		case TS_AC_RESULT_WITH_STRING::packetID:
		{
			const TS_AC_RESULT_WITH_STRING* resultMsg = reinterpret_cast<const TS_AC_RESULT_WITH_STRING*>(packetData);
			if(resultMsg->request_msg_id == TS_CA_ACCOUNT::packetID) {
				emit thisAccount->onLoginResult(resultMsg->result == 0, QByteArray(resultMsg->string, resultMsg->strSize));
			}
			break;
		}

		case TS_AC_SERVER_LIST::packetID:
			thisAccount->onPacketServerList(reinterpret_cast<const TS_AC_SERVER_LIST*>(packetData));
			break;

		case TS_AC_SELECT_SERVER::packetID:
			thisAccount->onPacketSelectServerResult(reinterpret_cast<const TS_AC_SELECT_SERVER*>(packetData));
			break;
	}
}

void Authentication::onGamePacketReceived(Server* server, const TS_MESSAGE* packetData) {
	Authentication* thisAccount = server->getAuth();

	switch(packetData->id) {
		case TS_SC_RESULT::packetID:
		{
			const TS_SC_RESULT* resultMsg = reinterpret_cast<const TS_SC_RESULT*>(packetData);
			if(resultMsg->request_msg_id == TS_CS_ACCOUNT_WITH_AUTH::packetID)
				thisAccount->onPacketGameAuthResult(resultMsg);
			break;
		}
	}
}


void Authentication::onPacketAuthConnected() {
	TS_CA_VERSION versionMsg;

	//Si ce paquet ne provient pas de notre initiative, on l'ignore
	if(currentState == AS_Idle)
		return;

	TS_MESSAGE::initMessage<TS_CA_VERSION>(&versionMsg);
	qstrcpy(versionMsg.szVersion, clientVersion.constData());
	server->sendPacket(&versionMsg, Server::ST_Auth);

	if(this->cipherMethod == ACM_DES) {
		TS_CA_ACCOUNT accountMsg;

		this->password.resize(61);
		DESPasswordCipher("MERONG").encrypt(this->password.data(), 61);

		TS_MESSAGE::initMessage<TS_CA_ACCOUNT>(&accountMsg);
		qstrcpy(accountMsg.account, username.constData());
		qstrcpy(accountMsg.password, password.constData());
		server->sendPacket(&accountMsg, Server::ST_Auth);
	} else if(this->cipherMethod == ACM_RSA_AES) {
		TS_CA_RSA_PUBLIC_KEY *keyMsg;
		int public_key_size;

		rsaCipher = RSA_generate_key(1024, 65537, NULL, NULL);

		BIO * b = BIO_new(BIO_s_mem());
		PEM_write_bio_RSA_PUBKEY(b, (RSA*)rsaCipher);

		public_key_size = BIO_get_mem_data(b, NULL);
		keyMsg = TS_MESSAGE_WNA::create<TS_CA_RSA_PUBLIC_KEY, unsigned char>(public_key_size);

		keyMsg->key_size = public_key_size;

		BIO_read(b, keyMsg->key, public_key_size);
		BIO_free(b);

		server->sendPacket(keyMsg, Server::ST_Auth);
		TS_MESSAGE_WNA::destroy(keyMsg);
	}
}

void Authentication::onPacketAuthUnreachable() {
	currentState = AS_Idle;
	emit onAuthConnectionFailed();
}

void Authentication::onPacketAuthPasswordKey(const TS_AC_AES_KEY_IV* packet) {
	TS_CA_ACCOUNT_V2 accountMsg;
	unsigned char decrypted_data[256];
	int data_size;
	bool ok = false;

	EVP_CIPHER_CTX e_ctx;
	const unsigned char *key_data = decrypted_data;
	const unsigned char *iv_data = decrypted_data + 16;
	int len = password.size();
	int p_len = len, f_len = 0;

	//Si ce paquet ne provient pas de notre initiative, on l'ignore
	if(currentState == AS_Idle)
		return;

	data_size = RSA_private_decrypt(packet->data_size, (unsigned char*)packet->rsa_encrypted_data, decrypted_data, (RSA*)rsaCipher, RSA_PKCS1_PADDING);
	RSA_free((RSA*)rsaCipher);
	rsaCipher = 0;
	if(data_size != 32) {
		qWarning(LOG_PREFIX"onPacketAuthPasswordKey: invalid decrypted data size: %d", data_size);
		//Si ce paquet ne provient pas de notre initiative, on l'ignore
		if(currentState == AS_ProcessLogin)
			server->close();
		return;
	}

	aes_key_iv = new unsigned char[32];
	qMemCopy(aes_key_iv, decrypted_data, 32);
	qMemSet(accountMsg.password, 0, sizeof(accountMsg.password));

	EVP_CIPHER_CTX_init(&e_ctx);

	if(!EVP_EncryptInit_ex(&e_ctx, EVP_aes_128_cbc(), NULL, key_data, iv_data))
		goto end;
	if(!EVP_EncryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL))
		goto end;
	if(!EVP_EncryptUpdate(&e_ctx, accountMsg.password, &p_len, (const unsigned char*)password.constData(), len))
		goto end;
	if(!EVP_EncryptFinal_ex(&e_ctx, accountMsg.password+p_len, &f_len))
		goto end;

	ok = true;

end:
	EVP_CIPHER_CTX_cleanup(&e_ctx);

	if(ok == false) {
		qWarning(LOG_PREFIX"onPacketAuthPasswordKey: could not encrypt password !");
		server->close();
		return;
	}

	password.fill(0);
	password.clear();

	TS_MESSAGE::initMessage<TS_CA_ACCOUNT_V2>(&accountMsg);
	qMemSet(accountMsg.account, 0, sizeof(accountMsg.account));
	qstrcpy(accountMsg.account, username.constData());
	accountMsg.aes_block_size = p_len + f_len;
	accountMsg.dummy[0] = accountMsg.dummy[1] = accountMsg.dummy[2] = 0;
	accountMsg.unknown_00000100 = 0x00000100;
	server->sendPacket(&accountMsg, Server::ST_Auth);

	char debugMsg[100];
	debugMsg[0] = 32+7;
	debugMsg[1] = 0;
	debugMsg[2] = 0;
	debugMsg[3] = 0;

	debugMsg[4] = 9999 & 0xFF;
	debugMsg[5] = (9999 >> 8) & 0xFF;
	debugMsg[6] = TS_MESSAGE::checkMessage((TS_MESSAGE*)&debugMsg);
	qMemCopy(debugMsg+7, decrypted_data, 32);
	server->sendPacket((TS_MESSAGE*)debugMsg, Server::ST_Auth);
}

void Authentication::onPacketServerList(const TS_AC_SERVER_LIST* packet) {
	QList<ServerInfo> serverList;
	ServerInfo currentServerInfo;
	ServerConnectionInfo serverConnectionInfo;
	const TS_AC_SERVER_LIST::TS_SERVER_INFO* packetServerList = packet->servers;

	serverList.reserve(packet->count);
	selectedServer = 0;
	this->serverList.clear();

	for(int i=0; i < packet->count; ++i) {
		currentServerInfo.serverId = packetServerList[i].server_idx;
		currentServerInfo.serverIp = QByteArray(packetServerList[i].server_ip);
		currentServerInfo.serverPort = packetServerList[i].server_port;
		currentServerInfo.serverName = QByteArray(packetServerList[i].server_name);
		currentServerInfo.serverScreenshotUrl = QByteArray(packetServerList[i].server_screenshot_url);
		currentServerInfo.userRatio = packetServerList[i].user_ratio;
		serverList.append(currentServerInfo);

		//Keep in memory server's ip & port for the server move
		serverConnectionInfo.id = currentServerInfo.serverId;
		serverConnectionInfo.ip = currentServerInfo.serverIp;
		serverConnectionInfo.port = currentServerInfo.serverPort;
		this->serverList.append(serverConnectionInfo);

		if(packetServerList[i].server_idx == packet->last_login_server_idx)
			selectedServer = i;
	}

	currentState = AS_Idle;
	emit onServerList(serverList, packet->last_login_server_idx);
}

void Authentication::onPacketSelectServerResult(const TS_AC_SELECT_SERVER* packet) {
	int pendingTimeBeforeServerMove;

	//Si ce paquet ne provient pas de notre initiative, on l'ignore
	if(currentState == AS_Idle) {
		ServerConnectionInfo selectedServerInfo = serverList.at(selectedServer);
		server->proceedServerMove(selectedServerInfo.ip, selectedServerInfo.port);
		return;
	}

	if(this->cipherMethod == ACM_DES) {
		oneTimePassword = packet->v1.one_time_key;
		pendingTimeBeforeServerMove = packet->v1.pending_time;
	} else if(this->cipherMethod == ACM_RSA_AES) {
		EVP_CIPHER_CTX e_ctx;
		const unsigned char *key_data = aes_key_iv;
		const unsigned char *iv_data = aes_key_iv + 16;
		int len = 16;
		int p_len = 0, f_len = 0;
		bool ok = false;
		quint64 decryptedData[2];

		EVP_CIPHER_CTX_init(&e_ctx);
		if(!EVP_DecryptInit_ex(&e_ctx, EVP_aes_128_cbc(), NULL, key_data, iv_data))
			goto end;
		if(!EVP_DecryptInit_ex(&e_ctx, NULL, NULL, NULL, NULL))
			goto end;
		if(!EVP_DecryptUpdate(&e_ctx, (unsigned char*)decryptedData, &p_len, packet->v2.encrypted_data, len))
			goto end;
		if(!EVP_DecryptFinal_ex(&e_ctx, ((unsigned char*)decryptedData) + p_len, &f_len))
			goto end;
		ok = true;
end:
		EVP_CIPHER_CTX_cleanup(&e_ctx);
		delete[] aes_key_iv;
		aes_key_iv = 0;

		if(ok == false) {
			qWarning(LOG_PREFIX"onPacketSelectServerResult: Could not decrypt TS_AC_SELECT_SERVER");
			server->close();
			return;
		}

		oneTimePassword = decryptedData[0];
		pendingTimeBeforeServerMove = packet->v2.pending_time;
	}

	ServerConnectionInfo selectedServerInfo = serverList.at(selectedServer);
	server->proceedServerMove(selectedServerInfo.ip, selectedServerInfo.port);
}

void Authentication::onPacketGameConnected() {
	TS_CS_VERSION versionMsg;
	TS_CS_ACCOUNT_WITH_AUTH loginInGameServerMsg;

	//Si ce paquet ne provient pas de notre initiative, on l'ignore
	if(currentState == AS_Idle)
		return;

	//continue server move as we are connected now to game server
	TS_MESSAGE::initMessage<TS_CS_VERSION>(&versionMsg);
	qstrcpy(versionMsg.szVersion, clientVersion.constData());
	server->sendPacket(&versionMsg, Server::ST_Game);

	TS_MESSAGE::initMessage<TS_CS_ACCOUNT_WITH_AUTH>(&loginInGameServerMsg);
	qstrcpy(loginInGameServerMsg.account, username.constData());
	loginInGameServerMsg.one_time_key = oneTimePassword;
	server->sendPacket(&loginInGameServerMsg, Server::ST_Game);
}

void Authentication::onPacketGameUnreachable() {
	currentState = AS_Idle;

	if(canBeDeleted == false)
		emit onGameConnectionFailed();

	canBeDeleted = true;
}

void Authentication::onPacketGameAuthResult(const TS_SC_RESULT* packet) {
	currentState = AS_Idle;

	if(canBeDeleted == false) {
		if(packet->result == 0)
			emit onGameServerReady();
		else
			emit onGameConnectionFailed();
	}

	canBeDeleted = true;
}
