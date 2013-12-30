#ifndef SERVER_H
#define SERVER_H

#include "Object.h"
#include "RappelzLib_global.h"
#include "IDelegate.h"

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
#include "ICallbackGuard.h"
#include "Socket.h"

/**
 * @brief Represent a set of servers.
 *
 * All network flows go through this class which redirect data to the correct server.
 * This class allow to connect to a rappelz server farm without many method calls.
 */
class RAPPELZLIB_EXTERN Server : public Object, private ICallbackGuard
{
	DECLARE_CLASS(Server)

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

		typedef void (*CallbackFunction)(ICallbackGuard* instance, Server* server, const TS_MESSAGE* packetData);

	private:
		static const uint32_t initialInputBufferSize = 16384;
		struct InputBuffer {
			uint8_t* buffer;
			uint32_t bufferSize;
			uint32_t currentMessageSize;
		};

	//Methods
	public:
		Server(uv_loop_t* uvLoop);
		virtual ~Server();

		//Set auth server that allow to connect to a world.
		void setServerFarm(const std::string &authHost, uint16_t authPort);

		void sendPacket(const TS_MESSAGE* data, ServerType destServer = ST_Game);
		void connectToAuth();
		void close();
		inline State getState() { return currentState; }

		void setAuth(Authentication *a) { auth = a; }
		Authentication* getAuth() { return auth; }

		void addPacketListener(ServerType server, uint16_t packetId, ICallbackGuard* instance, CallbackFunction onPacketReceivedCallback);
		void proceedServerMove(const std::string &gameHost, uint16_t gamePort);

	protected:
		static void networkDataReceivedFromAuth(ICallbackGuard* instance, Socket* socket);
		static void networkDataReceivedFromGame(ICallbackGuard* instance, Socket* socket);
		static void authStateChanged(ICallbackGuard* instance, Socket* socket, Socket::State oldState, Socket::State newState);
		static void gameStateChanged(ICallbackGuard* instance, Socket* socket, Socket::State oldState, Socket::State newState);
		static void authSocketError(ICallbackGuard* instance, Socket* socket, int errnoValue);
		static void gameSocketError(ICallbackGuard* instance, Socket* socket, int errnoValue);

		void networkDataProcess(ServerType serverType, EncryptedSocket* socket, InputBuffer* buffer);
		void dispatchPacket(ServerType originatingServer, const TS_MESSAGE* packetData);

	private:
		CallbacksTable *callbacks;
		EncryptedSocket* authSocket;
		EncryptedSocket* gameSocket;
		std::string authHost;
		std::string gameHost;
		uint16_t authPort;
		uint16_t gamePort;
		State currentState;
		Authentication *auth;

		InputBuffer authInputBuffer;
		InputBuffer gameInputBuffer;
};

#endif // SERVER_H
