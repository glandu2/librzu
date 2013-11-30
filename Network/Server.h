#ifndef SERVER_H
#define SERVER_H

#include "../Common/RappelzLib_global.h"
#include "../Interfaces/IDelegate.h"

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
#include "../Interfaces/ISocket.h"
#include "../Interfaces/ICallbackGuard.h"

/**
 * @brief Represent a set of servers.
 *
 * All network flows go through this class which redirect data to the correct server.
 * This class allow to connect to a rappelz server farm without many method calls.
 */
class RAPPELZLIBSHARED_EXPORT Server : private ICallbackGuard
{
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

		typedef void (*CallbackFunction)(void* instance, Server* server, const TS_MESSAGE* packetData);

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
		void setServerFarm(const std::string &authHost, quint16 authPort);

		void sendPacket(const TS_MESSAGE* data, ServerType destServer = ST_Game);
		void connectToAuth();
		void close();
		inline State getState() { return currentState; }

		void setAuth(Authentication *a) { auth = a; }
		Authentication* getAuth() { return auth; }

		DelegateInvalidatePtr addPacketListener(ServerType server, uint16_t packetId, void* instance, CallbackFunction onPacketReceivedCallback);
		void proceedServerMove(const std::string &gameHost, quint16 gamePort);

	protected:
		static void networkDataReceivedFromAuth(void* instance, ISocket* socket);
		static void networkDataReceivedFromGame(void* instance, ISocket* socket);
		static void authStateChanged(void* instance, ISocket* socket, ISocket::State oldState, ISocket::State newState);
		static void gameStateChanged(void* instance, ISocket* socket, ISocket::State oldState, ISocket::State newState);
		static void authSocketError(void* instance, ISocket* socket, int errnoValue);
		static void gameSocketError(void* instance, ISocket* socket, int errnoValue);

		void networkDataProcess(ServerType serverType, EncryptedSocket* socket, InputBuffer* buffer);
		void dispatchPacket(ServerType originatingServer, const TS_MESSAGE* packetData);

	private:
		CallbacksTable *callbacks;
		EncryptedSocket* authSocket;
		EncryptedSocket* gameSocket;
		std::string authHost;
		std::string gameHost;
		quint16 authPort;
		quint16 gamePort;
		State currentState;
		Authentication *auth;

		InputBuffer authInputBuffer;
		InputBuffer gameInputBuffer;
};

#endif // SERVER_H
