#include "Server.h"
#include "EncryptedSocket.h"
#include "../Packets/AuthPackets.h"
#include <openssl/evp.h>

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


	authSocket->addDataListener(this, &networkDataReceivedFromAuth);
	authSocket->addErrorListener(this, &authSocketError);
	authSocket->addEventListener(this, &authStateChanged);
	gameSocket->addDataListener(this, &networkDataReceivedFromGame);
	gameSocket->addErrorListener(this, &gameSocketError);
	gameSocket->addEventListener(this, &gameStateChanged);
}

Server::~Server() {
	close();
	delete authSocket;
	delete gameSocket;
	delete callbacks;
}

void Server::setServerFarm(const QByteArray& authHost, quint16 authPort) {
	currentState = SS_NotConnected;

//	authSocket->abort();
//	gameSocket->abort();

	this->authHost = authHost;
	this->authPort = authPort;
	this->gameHost = "undefined";
	this->gamePort = 0;

	if(authSocket->state() != QAbstractSocket::UnconnectedState && !authSocket->waitForDisconnected(1000))
		return;
	if(gameSocket->state() != QAbstractSocket::UnconnectedState && !gameSocket->waitForDisconnected(1000))
		return;
}

void Server::connectToAuth() {
	if(currentState == SS_NotConnected) {
		currentState = SS_ConnectingToAuth;
		qDebug("Connecting to auth: %s:%d", authHost.constData(), authPort);
		authSocket->connectToHost(QString::fromUtf8(authHost), authPort);
	} else qWarning("Attempt to connect to auth while not in SS_NotConnected mode, currentState: %d", currentState);
}

void Server::authStateChanged(void* instance, ISocket* socket, ISocket::State oldState, ISocket::State newState) {
	Server* thisInstance = static_cast<Server*>(instance);

	if(newState == ISocket::ConnectedState) {
		if(thisInstance->currentState == SS_ConnectingToAuth) {
			qDebug(LOG_PREFIX"Auth server %s:%d connected", thisInstance->authHost.constData(), thisInstance->authPort);

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
			qWarning(LOG_PREFIX"Auth server %s:%d connection lost !", thisInstance->authHost.constData(), thisInstance->authPort);
		} else {
			eventMsg.event = TS_CC_EVENT::CE_ServerDisconnected;
			qDebug(LOG_PREFIX"Auth server %s:%d disconnected", thisInstance->authHost.constData(), thisInstance->authPort);
		}

		thisInstance->dispatchPacket(ST_Auth, &eventMsg);
	}
}

void Server::gameStateChanged(void* instance, ISocket* socket, ISocket::State oldState, ISocket::State newState) {
	Server* thisInstance = static_cast<Server*>(instance);

	if(newState == ISocket::ConnectedState) {
		if(thisInstance->currentState == SS_ServerConnectionMove) {
			qDebug(LOG_PREFIX"Game server %s:%d connected", thisInstance->gameHost.constData(), thisInstance->gamePort);

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
			qDebug(LOG_PREFIX"Game server %s:%d connection lost !", thisInstance->gameHost.constData(), thisInstance->gamePort);
		} else {
			eventMsg.event = TS_CC_EVENT::CE_ServerDisconnected;
			qDebug(LOG_PREFIX"Game server %s:%d disconnected", thisInstance->gameHost.constData(), thisInstance->gamePort);
		}

		thisInstance->close();

		thisInstance->dispatchPacket(ST_Game, &eventMsg);
	}
}

void Server::authSocketError(void* instance, ISocket* socket, int errnoValue) {
	Server* thisInstance = static_cast<Server*>(instance);

	thisInstance->close();
	qWarning(LOG_PREFIX"Auth server %s:%d socket error %s !", thisInstance->authHost.constData(), thisInstance->authPort, strerror(errnoValue));

	TS_CC_EVENT eventMsg;
	TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
	eventMsg.event = TS_CC_EVENT::CE_ServerUnreachable;
	thisInstance->dispatchPacket(ST_Auth, &eventMsg);
}

void Server::gameSocketError(void* instance, ISocket* socket, int errnoValue) {
	Server* thisInstance = static_cast<Server*>(instance);

	thisInstance->close();
	qWarning(LOG_PREFIX"Game server %s:%d socket error %s !", thisInstance->gameHost.constData(), thisInstance->gamePort, strerror(errnoValue));

	TS_CC_EVENT eventMsg;
	TS_MESSAGE::initMessage<TS_CC_EVENT>(&eventMsg);
	eventMsg.event = TS_CC_EVENT::CE_ServerUnreachable;
	thisInstance->dispatchPacket(ST_Game, &eventMsg);
}

void Server::close() {
	currentState = SS_NotConnected;

	if(authSocket->state() != QAbstractSocket::UnconnectedState) {
		authSocket->close();
		if(authSocket->state() != QAbstractSocket::UnconnectedState)
			authSocket->waitForDisconnected(1000);
	}
	if(gameSocket->state() != QAbstractSocket::UnconnectedState) {
		gameSocket->close();
		if(gameSocket->state() != QAbstractSocket::UnconnectedState)
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

void Server::proceedServerMove(const QByteArray &gameHost, quint16 gamePort) {
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
	CallbackIterator it;

	if(originatingServer == ST_Auth) {
		if(packetData->id == TS_SC_RESULT::packetID)
			callbackFunctionsToCall = callbacks->authPacketListeners.equal_range(reinterpret_cast<const TS_SC_RESULT*>(packetData)->request_msg_id);
		else callbackFunctionsToCall = callbacks->authPacketListeners.equal_range(packetData->id);
	} else if(originatingServer == ST_Game)
		if(packetData->id == TS_SC_RESULT::packetID)
			callbackFunctionsToCall = callbacks->gamePacketListeners.equal_range(reinterpret_cast<const TS_SC_RESULT*>(packetData)->request_msg_id);
		else callbackFunctionsToCall = callbacks->gamePacketListeners.equal_range(packetData->id);
	else return;

	for(it = callbackFunctionsToCall.first; it != callbackFunctionsToCall.second; ++it) {
		const CallbacksTable::CallbackInfo& callbackInfo = it->second;
		(*callbackInfo.callback)(callbackInfo.instance, this, packetData);
	}
}

void Server::networkDataReceivedFromAuth(void* instance, ISocket* socket) {
	Server* thisInstance = static_cast<Server*>(instance);

	if(thisInstance->currentState == SS_ConnectedToAuth)
		thisInstance->networkDataProcess(ST_Auth, thisInstance->authSocket, &thisInstance->authInputBuffer);
	else qWarning(LOG_PREFIX"Received data from auth but not in SS_ConnectedToAuth mode, currentState: %d", thisInstance->currentState);
}

void Server::networkDataReceivedFromGame(void* instance, ISocket* socket) {
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

void Server::addPacketListener(ServerType server, uint16_t packetId, CallbackFunction onPacketReceivedCallback, void* arg) {
	CallbacksTable::CallbackInfo callback = {onPacketReceivedCallback, arg};

	if(server == ST_Auth)
		callbacks->authPacketListeners.emplace(packetId, callback);
	else if(server == ST_Game)
		callbacks->gamePacketListeners.emplace(packetId, callback);
	else qFatal(LOG_PREFIX"addPacketListener: Invalid server type: %d", server);
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
