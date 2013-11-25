#include "EncryptedSocket.h"
#include <stdio.h>
#include <malloc.h>
#include <QtConcurrentRun>

SocketPoll EncryptedSocket::socketPoll;

EncryptedSocket::EncryptedSocket() {
	addDataListener(this, &onDataReady);
	addErrorListener(this, &onError);
	addEventListener(this, &onStateChanged);

	static int r = 0;
	if(!r) {
		Socket::setPoll(&socketPoll);
		QtConcurrent::run(&updatePoll);
		r = 1;
	}
}

EncryptedSocket::~EncryptedSocket() {
	abort();
}

bool EncryptedSocket::connectToHost(const QString & hostName, quint16 port) {
	Socket::connect(hostName.toAscii().constData(), port);

    inputEnc.prepare("}h79q~B%al;k'y $E");
    outputEnc.prepare("}h79q~B%al;k'y $E");

	return true; //QTcpSocket::waitForConnected(10000);
}

qint64 EncryptedSocket::read(char *buffer, qint64 size) {
	qint64 ret = Socket::read((char*)buffer, size);
    inputEnc.encode((const char*)buffer, (char*)buffer, ret);

    return ret;
}

qint64 EncryptedSocket::write(const char *buffer, qint64 size) {
    char *encbuffer = (char*)alloca(size);
    qint64 ret;

    outputEnc.encode((const char*)buffer, encbuffer, size);
	ret = Socket::write(encbuffer, size);

    return ret;
}

void EncryptedSocket::onDataReady(void* instance, ISocket* socket) {
	EncryptedSocket* thisInstance = static_cast<EncryptedSocket*>(instance);
	emit thisInstance->readyRead();
}

void EncryptedSocket::onStateChanged(void* instance, ISocket* socket, State oldState, State newState) {
	EncryptedSocket* thisInstance = static_cast<EncryptedSocket*>(instance);

	if(newState == ConnectedState)
		emit thisInstance->connected();
	else if(oldState == ConnectedState)
		emit thisInstance->disconnected();
}

void EncryptedSocket::onError(void* instance, ISocket* socket, int errnoValue) {
	EncryptedSocket* thisInstance = static_cast<EncryptedSocket*>(instance);

	if(thisInstance->getState() == ConnectingState)
		emit thisInstance->error(QAbstractSocket::ConnectionRefusedError);
}

QAbstractSocket::SocketState EncryptedSocket::state() {
	switch(getState()) {
		case UnconnectedState:
			return QAbstractSocket::UnconnectedState;
		case Socket::HostLookupState:
			return QAbstractSocket::HostLookupState;
		case Socket::ConnectingState:
			return QAbstractSocket::ConnectingState;
		case Socket::ConnectedState:
			return QAbstractSocket::ConnectedState;
		case Socket::ClosingState:
			return QAbstractSocket::ClosingState;

		default:
			return QAbstractSocket::UnconnectedState;
	}
}

void EncryptedSocket::updatePoll() {
	socketPoll.run();
}
