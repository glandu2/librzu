#ifndef CLIENTAUTHSESSION_H
#define CLIENTAUTHSESSION_H

#include "Object.h"
#include "IListener.h"
#include <string>
#include <stdint.h>
#include <IDelegate.h>
#include "Packets/PacketEnums.h"
#include <vector>
#include "uv.h"
#include "DesPasswordCipher.h"
#include "DelegatedPacketSession.h"
#include "EncryptedSession.h"

struct TS_MESSAGE;
struct TS_AC_SERVER_LIST;
struct TS_AC_SELECT_SERVER;
struct TS_AC_AES_KEY_IV;
struct TS_SC_RESULT;

class ClientGameSession;

class RAPPELZLIB_EXTERN ClientAuthSession : protected EncryptedSession<PacketSession>
{
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
		enum AuthCipherMethod {
			ACM_DES,
			ACM_RSA_AES  //Since mid epic 8.1
		};

	public:
		ClientAuthSession(ClientGameSession* gameSession);
		~ClientAuthSession();

		using EncryptedSession<PacketSession>::connect;
		bool connect(const std::string& ip, uint16_t port, const std::string& account, const std::string& password, AuthCipherMethod method = ACM_DES, const std::string& version = "200701120");
		void close();

		void retreiveServerList();
		bool selectServer(uint16_t serverId);

		const std::string& getAccountName() { return username; }
		uint64_t getOnTimePassword() { return oneTimePassword; }
		const std::string& getVersion() { return version; }

	protected:
		virtual void onAuthDisconnected() = 0;
		virtual void onAuthResult(TS_ResultCode result, const std::string& resultString) = 0;
		virtual void onServerList(const std::vector<ServerInfo>& servers, uint16_t lastSelectedServerId) = 0;

		virtual void onGameDisconnected() = 0;
		virtual void onGameResult(TS_ResultCode result) = 0;

		friend class ClientGameSession;

	private:
		void onConnected();
		void onDisconnected();
		void onPacketAuthPasswordKey(const TS_AC_AES_KEY_IV *packet);
		void onPacketServerList(const TS_AC_SERVER_LIST* packet);
		void onPacketSelectServerResult(const TS_AC_SELECT_SERVER* packet);
		void onPacketGameAuthResult(const TS_SC_RESULT* packet);


	private:
		void onPacketReceived(const TS_MESSAGE* packetData);

	private:
		struct ServerConnectionInfo {
			uint16_t id;
			std::string ip;
			uint16_t port;
		};

	protected:
		ClientGameSession *gameSession;

	private:
		AuthCipherMethod cipherMethod;
		std::string version;
		std::string username;
		std::string password;
		unsigned char aes_key_iv[32];
		void* rsaCipher;
		std::vector<ServerConnectionInfo> serverList;
		int selectedServer;
		uint64_t oneTimePassword;
		bool normalDisconnect;

		static DesPasswordCipher desCipher;
};

#endif // CLIENTAUTHSESSION_H
