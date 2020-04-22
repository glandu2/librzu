#pragma once

#include "EncryptedSession.h"
#include "PacketEnums.h"
#include "PacketSession.h"
#include <stdint.h>
#include <string>
#include <vector>

struct TS_MESSAGE;
struct TS_AC_SERVER_LIST;
struct TS_AC_SELECT_SERVER;
struct TS_AC_AES_KEY_IV;
struct TS_SC_RESULT;
struct TS_AC_RESULT;
struct TS_AC_RESULT_WITH_STRING;

class ClientGameSession;
class DesPasswordCipher;
class RsaCipher;

class RZU_EXTERN ClientAuthSession : public EncryptedSession<PacketSession> {
	DECLARE_CLASS(ClientAuthSession)
public:
	struct ServerInfo {
		uint16_t serverId;
		std::string serverName;
		std::string serverScreenshotUrl;
		std::string serverIp;
		int32_t serverPort;
		uint16_t userRatio;
	};

public:
	ClientAuthSession(ClientGameSession* gameSession, int packetVersion);
	~ClientAuthSession();

	using EncryptedSession<PacketSession>::connect;
	bool connect(const std::string& ip, uint16_t port, const std::string& account, const std::string& password);
	void close();

	void retreiveServerList();
	bool selectServer(uint16_t serverId);

	const std::string& getAccountName() { return username; }
	uint64_t getOnTimePassword() { return oneTimePassword; }
	const std::string& getAuthVersion() { return authVersion; }
	const std::string& getGameVersion() { return gameVersion; }

protected:
	virtual void onAuthDisconnected(bool causedByRemote) = 0;
	virtual void onAuthResult(TS_ResultCode result, const std::string& resultString) = 0;
	virtual void onServerList(const std::vector<ServerInfo>& servers, uint16_t lastSelectedServerId) = 0;

	virtual void onGameDisconnected(bool causedByRemote) = 0;
	virtual void onGameResult(TS_ResultCode result) = 0;

	friend class ClientGameSession;

private:
	EventChain<SocketSession> onConnected();
	EventChain<SocketSession> onDisconnected(bool causedByRemote);
	void onPacketAuthPasswordKey(const TS_AC_AES_KEY_IV* packet);
	void onPacketServerList(const TS_AC_SERVER_LIST* packet);
	void onPacketSelectServerResult(const TS_AC_SELECT_SERVER* packet);
	void onPacketGameAuthResult(const TS_SC_RESULT* packet);
	void onPacketAuthResult(const TS_AC_RESULT* packet);
	void onPacketAuthStringResult(const TS_AC_RESULT_WITH_STRING* packet);

private:
	EventChain<PacketSession> onPacketReceived(const TS_MESSAGE* packetData);

private:
	struct ServerConnectionInfo {
		uint16_t id;
		std::string ip;
		uint16_t port;
	};

protected:
	ClientGameSession* gameSession;

private:
	enum AuthCipherMethod {
		ACM_DES,
		ACM_RSA_AES  // Since mid epic 8.1
	};

	AuthCipherMethod cipherMethod;
	std::string authVersion;
	std::string gameVersion;
	std::string username;
	std::string password;
	std::vector<uint8_t> aesKey;
	std::vector<ServerConnectionInfo> serverList;
	int selectedServer;
	uint64_t oneTimePassword;
	bool normalDisconnect;

	static DesPasswordCipher desCipher;
	static RsaCipher rsaCipher;
};

