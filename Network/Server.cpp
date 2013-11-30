#include "Server.h"
#include "EncryptedSocket.h"
#include "../Packets/AuthPackets.h"
#include <openssl/evp.h>
#include <string.h>

#include <unordered_map>

//#define NO_DEBUG

#define LOG_PREFIX "Server: "

#ifdef NO_DEBUG
# undef qDebug
# define qDebug(...) (void)0
#endif

struct CallbacksTable {
	struct CallbackInfo {
		Server::CallbackFunction callback;
		void* instance;
	};

	std::unordered_multimap<uint16_t, CallbackInfo > authPacketListeners;
	std::unordered_multimap<uint16_t, CallbackInfo > gamePacketListeners;
};

Server::Server() : callbacks(new CallbacksTable) {
	callbacks->authPacketListeners.reserve(15);
	callbacks->gamePacketListeners.reserve(250);

	authSocket = new EncryptedSocket;
	gameSocket = new EncryptedSocket;

	authInputBuffer.bufferSize = initialInputBufferSize;
	authInputBuffer.buffer = new uint8_t[authInputBuffer.bufferSize];
	authInputBuffer.currentMessageSize = 0;
	gameInputBuffer.bufferSize = initialInputBufferSize;
	gameInputBuffer.buffer = new uint8_t[gameInputBuffer.bufferSize];
	gameInputBuffer.currentMessageSize = 0;

	currentState = SS_NotConnected;


	addInstance(authSocket->addDataListener(this, &networkDataReceivedFromAuth));
	addInstance(authSocket->addErrorListener(this, &authSocketError));
	addInstance(authSocket->addEventListener(this, &authStateChanged));
	addInstance(gameSocket->addDataListener(this, &networkDataReceivedFromGame));
	addInstance(gameSocket->addErrorListener(this, &gameSocketError));
	addInstance(gameSocket->addEventListener(this, &gameStateChanged));
}

Server::~Server() {
	invalidateCallbacks();
	close();
	delete authSocket;
	delete gameSocket;
	delete callbacks;
}

void Server::setServerFarm(const std::string& authHost, quint16 authPort) {
	currentState = SS_NotConnected;

//	authSocket->abort();
//	gameSocket->abort();

	this->authHost = authHost;
	this->authPort = authPort;
	this->gameHost = "undefined";
	this->gamePort = 0;

	if(authSocket->getState() != ISocket::UnconnectedState && !authSocket->waitForDisconnected(1000))
		return;
	if(gameSocket->getState() != ISocket::UnconnectedState && !gameSocket->waitForDisconnected(1000))
		return;
}

void Server::connectToAuth() {
	if(currentState == SS_NotConnected) {
		currentState = SS_ConnectingToAuth;
		qDebug("Connecting to auth: %s:%d", authHost.c_str(), authPort);
		authSocket->connectToHost(authHost, authPort);
	} else qWarning("Attempt to connect to auth while not in SS_NotConnected mode, currentState: %d", currentState);
}

void Server::authStateChanged(void* instance, ISocket*, ISocket::State oldState, ISocket::State newState) {
	Server* thisInstance = static_cast<Server*>(instance);

	if(newState == ISocket::ConnectedState) {
		if(thisInstance->currentState == SS_ConnectingToAuth) {
			qDebug(LOG_PREFIX"Auth server %s:%d connected", thisInstance->authHost.c_str(), thisInstance->authPort);

			thisInstance->currentState = SS_ConnectedToAuth;
			thisInstance->authInputBuffer.currentMessageSize = 0;

			TS_CC_EVENT eventMsg;
			TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
			eventMsg.event = TS_CC_EVENT::CE_ServerConnected;
			thisInstance->dispatchPacket(ST_Auth, &eventMsg);
		} else {
			qWarning(LOG_PREFIX"Auth server connected, but not in SS_ConnectingToAuth mode !, currentState: %d", thisInstance->currentState);
		}
	} else if(oldState == ISocket::ConnectedState) {
		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		if(thisInstance->currentState == SS_ConnectedToAuth) {
			eventMsg.event = TS_CC_EVENT::CE_ServerConnectionLost;
			qWarning(LOG_PREFIX"Auth server %s:%d connection lost !", thisInstance->authHost.c_str(), thisInstance->authPort);
		} else {
			eventMsg.event = TS_CC_EVENT::CE_ServerDisconnected;
			qDebug(LOG_PREFIX"Auth server %s:%d disconnected", thisInstance->authHost.c_str(), thisInstance->authPort);
		}

		thisInstance->dispatchPacket(ST_Auth, &eventMsg);
	}
}

void Server::gameStateChanged(void* instance, ISocket*, ISocket::State oldState, ISocket::State newState) {
	Server* thisInstance = static_cast<Server*>(instance);

	if(newState == ISocket::ConnectedState) {
		if(thisInstance->currentState == SS_ServerConnectionMove) {
			qDebug(LOG_PREFIX"Game server %s:%d connected", thisInstance->gameHost.c_str(), thisInstance->gamePort);

			thisInstance->currentState = SS_ConnectedToGame;
			thisInstance->gameInputBuffer.currentMessageSize = 0;

			TS_CC_EVENT eventMsg;
			TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
			eventMsg.event = TS_CC_EVENT::CE_ServerConnected;
			thisInstance->dispatchPacket(ST_Game, &eventMsg);
		} else {
			qWarning(LOG_PREFIX"Game server connected, but not in SS_ServerConnectionMove mode !, currentState: %d", thisInstance->currentState);
		}
	} else if(oldState == ISocket::ConnectedState) {
		TS_CC_EVENT eventMsg;
		TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
		if(thisInstance->currentState != SS_NotConnected) {
			eventMsg.event = TS_CC_EVENT::CE_ServerConnectionLost;
			qDebug(LOG_PREFIX"Game server %s:%d connection lost !", thisInstance->gameHost.c_str(), thisInstance->gamePort);
		} else {
			eventMsg.event = TS_CC_EVENT::CE_ServerDisconnected;
			qDebug(LOG_PREFIX"Game server %s:%d disconnected", thisInstance->gameHost.c_str(), thisInstance->gamePort);
		}

		thisInstance->close();

		thisInstance->dispatchPacket(ST_Game, &eventMsg);
	}
}

void Server::authSocketError(void* instance, ISocket*, int errnoValue) {
	Server* thisInstance = static_cast<Server*>(instance);

	thisInstance->close();
	qWarning(LOG_PREFIX"Auth server %s:%d socket error %s !", thisInstance->authHost.c_str(), thisInstance->authPort, strerror(errnoValue));

	TS_CC_EVENT eventMsg;
	TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
	eventMsg.event = TS_CC_EVENT::CE_ServerUnreachable;
	thisInstance->dispatchPacket(ST_Auth, &eventMsg);
}

void Server::gameSocketError(void* instance, ISocket*, int errnoValue) {
	Server* thisInstance = static_cast<Server*>(instance);

	thisInstance->close();
	qWarning(LOG_PREFIX"Game server %s:%d socket error %s !", thisInstance->gameHost.c_str(), thisInstance->gamePort, strerror(errnoValue));

	TS_CC_EVENT eventMsg;
	TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
	eventMsg.event = TS_CC_EVENT::CE_ServerUnreachable;
	thisInstance->dispatchPacket(ST_Game, &eventMsg);
}

void Server::close() {
	currentState = SS_NotConnected;

	if(authSocket->getState() != ISocket::UnconnectedState) {
		authSocket->close();
		if(authSocket->getState() != ISocket::UnconnectedState)
			authSocket->waitForDisconnected(1000);
	}
	if(gameSocket->getState() != ISocket::UnconnectedState) {
		gameSocket->close();
		if(gameSocket->getState() != ISocket::UnconnectedState)
			gameSocket->waitForDisconnected(1000);
	}
}

void Server::sendPacket(const TS_MESSAGE* data, ServerType destServer) {
	qDebug(LOG_PREFIX"Packet to   %d, id: %5d, size: %d", destServer, data->id, data->size);

	if(destServer == ST_Auth && currentState == SS_ConnectedToAuth) {
		authSocket->write(data, data->size);
	} else if(currentState == SS_ConnectedToGame)
		gameSocket->write(data, data->size);
	else qFatal(LOG_PREFIX"sendPacket: invalid state: %d", currentState);
}

void Server::proceedServerMove(const std::string &gameHost, quint16 gamePort) {
	currentState = SS_ServerConnectionMove;

	authSocket->close();

	this->gameHost = gameHost;
	this->gamePort = gamePort;
	gameSocket->connectToHost(gameHost, gamePort);
}

void Server::dispatchPacket(ServerType originatingServer, const TS_MESSAGE* packetData) {
	qDebug(LOG_PREFIX"Packet from %d, id: %5d, size: %d", originatingServer, packetData->id, packetData->size);

	typedef std::unordered_multimap<uint16_t, CallbacksTable::CallbackInfo >::const_iterator CallbackIterator;

	std::pair<CallbackIterator, CallbackIterator> callbackFunctionsToCall;
	std::unordered_multimap<uint16_t, CallbacksTable::CallbackInfo > packetListeners;
	CallbackIterator it;

	if(originatingServer == ST_Auth) {
		packetListeners = callbacks->authPacketListeners;
	} else if(originatingServer == ST_Game) {
		packetListeners = callbacks->gamePacketListeners;
	} else {
		return;
	}

	if(packetData->id == TS_SC_RESULT::packetID)
		callbackFunctionsToCall = packetListeners.equal_range(reinterpret_cast<const TS_SC_RESULT*>(packetData)->request_msg_id);
	else callbackFunctionsToCall = packetListeners.equal_range(packetData->id);

	for(it = callbackFunctionsToCall.first; it != callbackFunctionsToCall.second;) {
		const CallbacksTable::CallbackInfo& callbackInfo = it->second;
		if(callbackInfo.callback != nullptr) {
			(*callbackInfo.callback)(callbackInfo.instance, this, packetData);
			++it;
		} else {
			it = packetListeners.erase(it);
		}
	}
}

void Server::networkDataReceivedFromAuth(void* instance, ISocket*) {
	Server* thisInstance = static_cast<Server*>(instance);

	if(thisInstance->currentState == SS_ConnectedToAuth)
		thisInstance->networkDataProcess(ST_Auth, thisInstance->authSocket, &thisInstance->authInputBuffer);
	else qWarning(LOG_PREFIX"Received data from auth but not in SS_ConnectedToAuth mode, currentState: %d", thisInstance->currentState);
}

void Server::networkDataReceivedFromGame(void* instance, ISocket*) {
	Server* thisInstance = static_cast<Server*>(instance);

	if(thisInstance->currentState == SS_ServerConnectionMove || thisInstance->currentState == SS_ConnectedToGame)
		thisInstance->networkDataProcess(ST_Game, thisInstance->gameSocket, &thisInstance->gameInputBuffer);
	else qWarning(LOG_PREFIX"Received data from game but not in SS_ServerConnectionMove or SS_ConnectedToGame mode, currentState: %d", thisInstance->currentState);
}


void Server::networkDataProcess(ServerType serverType, EncryptedSocket* socket, InputBuffer* buffer) {
	//qDebug() << LOG_PREFIX << "Server: " << serverType << " Received data, size = " << socket->bytesAvailable() << "CurrentSize = " << buffer->currentMessageSize;
	do {
		if(buffer->currentMessageSize == 0 && socket->bytesAvailable() < 4) {
			return;
		} else if(buffer->currentMessageSize == 0) {
			socket->read(&buffer->currentMessageSize, 4);
			//qDebug() << LOG_PREFIX << "New message received, size = " << buffer->currentMessageSize;
		}

		if(buffer->currentMessageSize != 0 && socket->bytesAvailable() >= (buffer->currentMessageSize - 4)) {
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
	} while((buffer->currentMessageSize == 0 && socket->bytesAvailable() >= 4) || (buffer->currentMessageSize != 0 && socket->bytesAvailable() >= (buffer->currentMessageSize - 4)));
}

ICallbackGuard::CallbackPtr Server::addPacketListener(ServerType server, uint16_t packetId, CallbackFunction onPacketReceivedCallback, void* arg) {
	CallbacksTable::CallbackInfo callback = {onPacketReceivedCallback, arg};
	std::unordered_multimap<uint16_t, CallbacksTable::CallbackInfo >::iterator it;

	if(server == ST_Auth)
		it = callbacks->authPacketListeners.emplace(packetId, callback);
	else if(server == ST_Game)
		it = callbacks->gamePacketListeners.emplace(packetId, callback);
	else {
		qFatal(LOG_PREFIX"addPacketListener: Invalid server type: %d", server);
		return nullptr;
	}

	return (ICallbackGuard::CallbackPtr)&(it->second.callback);
}

void Server::removePacketListener(ServerType serverType, uint16_t packetId, void *instance) {

	typedef std::unordered_multimap<uint16_t, CallbacksTable::CallbackInfo >::const_iterator CallbackIterator;

	std::unordered_multimap<uint16_t, CallbacksTable::CallbackInfo >& packetListeners = (serverType == ST_Auth)? callbacks->authPacketListeners : callbacks->gamePacketListeners;
	std::pair<CallbackIterator, CallbackIterator> callbackFunctions;
	CallbackIterator it;

	callbackFunctions = packetListeners.equal_range(packetId);

	for(it = callbackFunctions.first; it != callbackFunctions.second;) {
		const CallbacksTable::CallbackInfo& callbackInfo = it->second;
		if(callbackInfo.instance == instance)
			it = packetListeners.erase(it);
		else
			++it;
	}
}
