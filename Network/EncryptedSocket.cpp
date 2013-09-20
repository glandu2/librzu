#include "EncryptedSocket.h"
#include <stdio.h>
#include <malloc.h>

EncryptedSocket::EncryptedSocket() {
}

EncryptedSocket::~EncryptedSocket() {
    close();
}

bool EncryptedSocket::connectToHost(const QString & hostName, quint16 port, OpenMode openMode) {
    QTcpSocket::connectToHost(hostName, port, openMode);

    inputEnc.prepare("}h79q~B%al;k'y $E");
    outputEnc.prepare("}h79q~B%al;k'y $E");

	return true; //QTcpSocket::waitForConnected(10000);
}

qint64 EncryptedSocket::read(char *buffer, qint64 size) {
    qint64 ret = QTcpSocket::read((char*)buffer, size);
    inputEnc.encode((const char*)buffer, (char*)buffer, ret);

    return ret;
}

qint64 EncryptedSocket::write(const char *buffer, qint64 size) {
    char *encbuffer = (char*)alloca(size);
    qint64 ret;

    outputEnc.encode((const char*)buffer, encbuffer, size);
    ret = QTcpSocket::write(encbuffer, size);

    return ret;
}
