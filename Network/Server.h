#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "../Common/RappelzLib_global.h"

#if __cplusplus >= 201103L
#include "../Common/Delegate.h"
#endif

//Pour le syntax higlighting de intxx_t
#ifdef __GNUC__
# include <stdint-gcc.h>
#else
# include <stdint.h>
#endif

class EncryptedSocket;
struct TS_MESSAGE;
struct TS_AC_SERVER_LIST;
class Authentication;

struct CallbacksTable;

/**
 * @brief Represent a set of servers.
 *
 * All network flows go through this class which redirect data to the correct server.
 * This class allow to connect to a rappelz server farm without many method calls.
 */
class RAPPELZLIBSHARED_EXPORT Server : public QObject
{
	Q_OBJECT

	//Types
	public:
		enum State {
			SS_NotConnected,
			SS_ConnectingToAuth,
			SS_ConnectedToAuth,
			SS_ServerConnectionMove,
			SS_ConnectedToGame
		};
		enum ServerType {
			ST_Auth,
			ST_Game
		};


		typedef void (*CallbackFunction)(Server* server, const TS_MESSAGE* packetData);
		typedef void (*CallbackFunctionWithArg)(Server* server, const TS_MESSAGE* packetData, void* arg);

	private:
		static const quint32 initialInputBufferSize = 16384;
		struct InputBuffer {
			uint8_t* buffer;
			uint32_t bufferSize;
			uint32_t currentMessageSize;
		};

	//Methods
	public:
		Server();
		virtual ~Server();

		//Set auth server that allow to connect to a world.
		void setServerFarm(const QByteArray &authHost, quint16 authPort);

		void sendPacket(const TS_MESSAGE* data, ServerType destServer = ST_Game);
		void connectToAuth();
		void close();
		inline State getState() { return currentState; }

		inline void setAuth(Authentication *newAuth) { auth = newAuth; }
		inline Authentication* getAuth() { return auth; }

		int getInstanceId() { return instanceId; }

		void addPacketListener(ServerType server, uint16_t packetId, CallbackFunction onPacketReceivedCallback);
		void addPacketListener(ServerType server, uint16_t packetId, CallbackFunctionWithArg onPacketReceivedCallback, void* arg);
#if __cplusplus >= 201103L
#define server_callback(instance, method) [instance](Server* server, const TS_MESSAGE* packetData) { instance->method(server, packetData); }
		void addPacketListener(ServerType server, uint16_t packetId, cpp11x::function<void(Server*, const TS_MESSAGE*)> onPacketReceivedCallback);
#endif

	public slots:
		void proceedServerMove(const QByteArray &gameHost, quint16 gamePort);

	protected slots:
		void networkDataReceivedFromAuth();
		void networkDataReceivedFromGame();
		void authConnected();
		void gameConnected();
		void authDisconnected();
		void gameDisconnected();
		void authSocketError();
		void gameSocketError();

	protected:
		void networkDataProcess(ServerType serverType, EncryptedSocket* socket, InputBuffer* buffer);
		void dispatchPacket(ServerType originatingServer, const TS_MESSAGE* packetData);

	private:
		CallbacksTable *callbacks;
		int instanceId;
		EncryptedSocket* authSocket;
		EncryptedSocket* gameSocket;
		QByteArray authHost;
		QByteArray gameHost;
		quint16 authPort;
		quint16 gamePort;
		State currentState;
		Authentication* auth;

		InputBuffer authInputBuffer;
		InputBuffer gameInputBuffer;
};

#endif // SERVER_H
