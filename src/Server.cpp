#include "Server.h"
#include "EncryptedSocket.h"
#include "Packets/AuthPackets.h"
#include <string.h>

//#define NO_DEBUG

#define LOG_PREFIX "Server: "

#ifdef NO_DEBUG
# undef qDebug
# define qDebug(...) (void)0
#endif

struct CallbacksTable {
	IDelegateHash<uint16_t, Server::CallbackFunction> authPacketListeners;
	IDelegateHash<uint16_t, Server::CallbackFunction> gamePacketListeners;
};

Server::Server(uv_loop_t* uvLoop) : callbacks(new CallbacksTable) {
	callbacks->authPacketListeners.reserve(15);
	callbacks->gamePacketListeners.reserve(250);

	authSocket = new EncryptedSocket(uvLoop);
	gameSocket = new EncryptedSocket(uvLoop);

	authInputBuffer.bufferSize = initialInputBufferSize;
	authInputBuffer.buffer = new uint8_t[authInputBuffer.bufferSize];
	authInputBuffer.currentMessageSize = 0;
	gameInputBuffer.bufferSize = initialInputBufferSize;
	gameInputBuffer.buffer = new uint8_t[gameInputBuffer.bufferSize];
	gameInputBuffer.currentMessageSize = 0;

	currentState = SS_NotConnected;


	authSocket->addDataListener(this, &networkDataReceivedFromAuth);
	authSocket->addErrorListener(this, &authSocketError);
	authSocket->addEventListener(this, &authStateChanged);
	gameSocket->addDataListener(this, &networkDataReceivedFromGame);
	gameSocket->addErrorListener(this, &gameSocketError);
	gameSocket->addEventListener(this, &gameStateChanged);
}

Server::~Server() {
	invalidateCallbacks();
	close();
	delete authSocket;
	delete gameSocket;
	delete callbacks;
}

void Server::setServerFarm(const std::string& authHost, uint16_t authPort) {
	currentState = SS_NotConnected;

//	authSocket->abort();
//	gameSocket->abort();

	this->authHost = authHost;
	this->authPort = authPort;
	this->gameHost = "undefined";
	this->gamePort = 0;

	if(authSocket->getState() != Socket::UnconnectedState)
		return;
	if(gameSocket->getState() != Socket::UnconnectedState)
		return;
}

void Server::connectToAuth() {
	if(currentState == SS_NotConnected) {
		currentState = SS_ConnectingToAuth;
		printf("Connecting to auth: %s:%d\n", authHost.c_str(), authPort);
		authSocket->connect(authHost, authPort);
	} else printf("Attempt to connect to auth while not in SS_NotConnected mode, currentState: %d\n", currentState);
}

void Server::authStateChanged(ICallbackGuard* instance, Socket*, Socket::State oldState, Socket::State newState) {
	Server* thisInstance = static_cast<Server*>(instance);

	if(newState == Socket::ConnectedState) {
		if(thisInstance->currentState == SS_ConnectingToAuth) {
			printf(LOG_PREFIX"Auth server %s:%d connected\n", thisInstance->authHost.c_str(), thisInstance->authPort);

			thisInstance->currentState = SS_ConnectedToAuth;
			thisInstance->authInputBuffer.currentMessageSize = 0;

			TS_CC_EVENT eventMsg;
			TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
			eventMsg.event = TS_CC_EVENT::CE_ServerConnected;
			thisInstance->dispatchPacket(ST_Auth, &eventMsg);
		} else {
			printf(LOG_PREFIX"Auth server connected, but not in SS_ConnectingToAuth mode !, currentState: %d\n", thisInstance->currentState);
		}
	} else if(oldState == Socket::ConnectedState) {
		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		if(thisInstance->currentState == SS_ConnectedToAuth) {
			eventMsg.event = TS_CC_EVENT::CE_ServerConnectionLost;
			printf(LOG_PREFIX"Auth server %s:%d connection lost !\n", thisInstance->authHost.c_str(), thisInstance->authPort);
		} else {
			eventMsg.event = TS_CC_EVENT::CE_ServerDisconnected;
			printf(LOG_PREFIX"Auth server %s:%d disconnected\n", thisInstance->authHost.c_str(), thisInstance->authPort);
		}

		thisInstance->dispatchPacket(ST_Auth, &eventMsg);
	}
}

void Server::gameStateChanged(ICallbackGuard* instance, Socket*, Socket::State oldState, Socket::State newState) {
	Server* thisInstance = static_cast<Server*>(instance);

	if(newState == Socket::ConnectedState) {
		if(thisInstance->currentState == SS_ServerConnectionMove) {
			printf(LOG_PREFIX"Game server %s:%d connected\n", thisInstance->gameHost.c_str(), thisInstance->gamePort);

			thisInstance->currentState = SS_ConnectedToGame;
			thisInstance->gameInputBuffer.currentMessageSize = 0;

			TS_CC_EVENT eventMsg;
			TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
			eventMsg.event = TS_CC_EVENT::CE_ServerConnected;
			thisInstance->dispatchPacket(ST_Game, &eventMsg);
		} else {
			printf(LOG_PREFIX"Game server connected, but not in SS_ServerConnectionMove mode !, currentState: %d\n", thisInstance->currentState);
		}
	} else if(oldState == Socket::ConnectedState) {
		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		if(thisInstance->currentState != SS_NotConnected) {
			eventMsg.event = TS_CC_EVENT::CE_ServerConnectionLost;
			printf(LOG_PREFIX"Game server %s:%d connection lost !\n", thisInstance->gameHost.c_str(), thisInstance->gamePort);
		} else {
			eventMsg.event = TS_CC_EVENT::CE_ServerDisconnected;
			printf(LOG_PREFIX"Game server %s:%d disconnected\n", thisInstance->gameHost.c_str(), thisInstance->gamePort);
		}

		thisInstance->close();

		thisInstance->dispatchPacket(ST_Game, &eventMsg);
	}
}

void Server::authSocketError(ICallbackGuard* instance, Socket*, int errnoValue) {
	Server* thisInstance = static_cast<Server*>(instance);

	thisInstance->close();
	printf(LOG_PREFIX"Auth server %s:%d socket error %s !\n", thisInstance->authHost.c_str(), thisInstance->authPort, strerror(errnoValue));

	TS_CC_EVENT eventMsg;
	TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
	eventMsg.event = TS_CC_EVENT::CE_ServerUnreachable;
	thisInstance->dispatchPacket(ST_Auth, &eventMsg);
}

void Server::gameSocketError(ICallbackGuard* instance, Socket*, int errnoValue) {
	Server* thisInstance = static_cast<Server*>(instance);

	thisInstance->close();
	printf(LOG_PREFIX"Game server %s:%d socket error %s !\n", thisInstance->gameHost.c_str(), thisInstance->gamePort, strerror(errnoValue));

	TS_CC_EVENT eventMsg;
	TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
	eventMsg.event = TS_CC_EVENT::CE_ServerUnreachable;
	thisInstance->dispatchPacket(ST_Game, &eventMsg);
}

void Server::close() {
	currentState = SS_NotConnected;

	if(authSocket->getState() != Socket::UnconnectedState) {
		authSocket->close();
	}
	if(gameSocket->getState() != Socket::UnconnectedState) {
		gameSocket->close();
	}
}

void Server::sendPacket(const TS_MESSAGE* data, ServerType destServer) {
	printf(LOG_PREFIX"Packet to   %d, id: %5d, size: %d\n", destServer, data->id, data->size);

	if(destServer == ST_Auth && currentState == SS_ConnectedToAuth) {
		authSocket->write(data, data->size);
	} else if(currentState == SS_ConnectedToGame)
		gameSocket->write(data, data->size);
	else printf(LOG_PREFIX"sendPacket: invalid state: %d\n", currentState);
}

void Server::proceedServerMove(const std::string &gameHost, uint16_t gamePort) {
	currentState = SS_ServerConnectionMove;

	authSocket->close();

	this->gameHost = gameHost;
	this->gamePort = gamePort;
	gameSocket->connect(gameHost, gamePort);
}

void Server::dispatchPacket(ServerType originatingServer, const TS_MESSAGE* packetData) {
	IDelegateHash<uint16_t, Server::CallbackFunction> packetListeners;

	printf(LOG_PREFIX"Packet from %d, id: %5d, size: %d\n", originatingServer, packetData->id, packetData->size);

	if(originatingServer == ST_Auth) {
		packetListeners = callbacks->authPacketListeners;
	} else if(originatingServer == ST_Game) {
		packetListeners = callbacks->gamePacketListeners;
	} else {
		return;
	}

	if(packetData->id != TS_SC_RESULT::packetID)
		DELEGATE_HASH_CALL(packetListeners, packetData->id, this, packetData);
	else
		DELEGATE_HASH_CALL(packetListeners, reinterpret_cast<const TS_SC_RESULT*>(packetData)->request_msg_id, this, packetData);
}

void Server::networkDataReceivedFromAuth(ICallbackGuard* instance, Socket*) {
	Server* thisInstance = static_cast<Server*>(instance);

	if(thisInstance->currentState == SS_ConnectedToAuth)
		thisInstance->networkDataProcess(ST_Auth, thisInstance->authSocket, &thisInstance->authInputBuffer);
	else printf(LOG_PREFIX"Received data from auth but not in SS_ConnectedToAuth mode, currentState: %d\n", thisInstance->currentState);
}

void Server::networkDataReceivedFromGame(ICallbackGuard* instance, Socket*) {
	Server* thisInstance = static_cast<Server*>(instance);

	if(thisInstance->currentState == SS_ServerConnectionMove || thisInstance->currentState == SS_ConnectedToGame)
		thisInstance->networkDataProcess(ST_Game, thisInstance->gameSocket, &thisInstance->gameInputBuffer);
	else printf(LOG_PREFIX"Received data from game but not in SS_ServerConnectionMove or SS_ConnectedToGame mode, currentState: %d\n", thisInstance->currentState);
}


void Server::networkDataProcess(ServerType serverType, EncryptedSocket* socket, InputBuffer* buffer) {
	//qDebug() << LOG_PREFIX << "Server: " << serverType << " Received data, size = " << socket->bytesAvailable() << "CurrentSize = " << buffer->currentMessageSize;
	do {
		if(buffer->currentMessageSize == 0 && socket->getAvailableBytes() < 4) {
			return;
		} else if(buffer->currentMessageSize == 0) {
			socket->read(&buffer->currentMessageSize, 4);
			//qDebug() << LOG_PREFIX << "New message received, size = " << buffer->currentMessageSize;
		}

		if(buffer->currentMessageSize != 0 && socket->getAvailableBytes() >= (buffer->currentMessageSize - 4)) {
			if(buffer->currentMessageSize > buffer->bufferSize) {
				if(buffer->bufferSize)
					delete[] buffer->buffer;
				buffer->bufferSize = buffer->currentMessageSize;
				buffer->buffer = new uint8_t[buffer->currentMessageSize];
			}
			reinterpret_cast<TS_MESSAGE*>(buffer->buffer)->size = buffer->currentMessageSize;
			socket->read(buffer->buffer + 4, buffer->currentMessageSize - 4);
			dispatchPacket(serverType, reinterpret_cast<TS_MESSAGE*>(buffer->buffer));

			buffer->currentMessageSize = 0;
		}
	} while((buffer->currentMessageSize == 0 && socket->getAvailableBytes() >= 4) || (buffer->currentMessageSize != 0 && socket->getAvailableBytes() >= (buffer->currentMessageSize - 4)));
}

void Server::addPacketListener(ServerType server, uint16_t packetId, ICallbackGuard* instance, CallbackFunction onPacketReceivedCallback) {
	if(server == ST_Auth)
		callbacks->authPacketListeners.add(packetId, instance, onPacketReceivedCallback);
	else if(server == ST_Game)
		callbacks->gamePacketListeners.add(packetId, instance, onPacketReceivedCallback);
	else {
		printf(LOG_PREFIX"addPacketListener: Invalid server type: %d\n", server);
	}
}
