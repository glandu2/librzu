#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QObject>
#include "../Common/RappelzLib_global.h"

class Server;

struct TS_MESSAGE;
struct TS_AC_SERVER_LIST;
struct TS_AC_SELECT_SERVER;
struct TS_AC_AES_KEY_IV;
struct TS_SC_RESULT;

class RAPPELZLIBSHARED_EXPORT Authentication : public QObject
{
	Q_OBJECT
	public:
		struct ServerInfo {
			quint16 serverId;
			QByteArray serverName;
			QByteArray serverScreenshotUrl;
			QByteArray serverIp;
			qint32 serverPort;
			quint16 userRatio;
		};
		enum AuthCipherMethod {
			ACM_DES,
			ACM_RSA_AES  //Since mid epic 8.1
		};

	public:
		Authentication(Server* server);
		~Authentication();

		QByteArray getAccountName() { return username; }


	signals:
		void onAuthConnectionFailed();
		void onLoginResult(bool ok, QByteArray str = QByteArray());
		void onServerList(QList<Authentication::ServerInfo> serverList, quint16 lastSelectedServerId);
		void onGameServerReady();
		void onGameConnectionFailed();

	public slots:
		void setServer(QByteArray host, quint16 port = 4500);
		void connectAccount(QByteArray username, QByteArray password, QByteArray clientVersion, AuthCipherMethod cipherMethod);
		void abortConnection();
		void retreiveServerList();
		void selectServer(quint16 serverId);

	protected:
		static void onAuthServerConnectionEvent(Server* server, const TS_MESSAGE* packetData);
		static void onGameServerConnectionEvent(Server* server, const TS_MESSAGE* packetData);
		static void onAuthPacketReceived(Server* server, const TS_MESSAGE* packetData);
		static void onGamePacketReceived(Server* server, const TS_MESSAGE* packetData);

	protected:
		void onPacketAuthConnected();
		void onPacketAuthUnreachable();
		void onPacketAuthPasswordKey(const TS_AC_AES_KEY_IV *packet);
		void onPacketServerList(const TS_AC_SERVER_LIST* packet);
		void onPacketSelectServerResult(const TS_AC_SELECT_SERVER* packet);
		void onPacketGameConnected();
		void onPacketGameUnreachable();
		void onPacketGameAuthResult(const TS_SC_RESULT* packet);

	private:
		struct ServerConnectionInfo {
			quint16 id;
			QByteArray ip;
			quint16 port;
		};
		enum State {
			AS_Idle,
			AS_ProcessLogin,
			AS_ProcessServerList,
			AS_ProcessServerMove
		};

	private:
		Server* server;
		unsigned char* aes_key_iv;
		QByteArray username;
		QByteArray password;
		QByteArray clientVersion;
		QList<ServerConnectionInfo> serverList;
		quint16 selectedServer;
		quint64 oneTimePassword;
		AuthCipherMethod cipherMethod;
		bool canBeDeleted;
		void* rsaCipher;

		State currentState;
};

#endif // AUTHENTICATION_H
