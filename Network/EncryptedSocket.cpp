#include "EncryptedSocket.h"
#include <stdio.h>
#include <malloc.h>
#include <thread>

SocketPoll EncryptedSocket::socketPoll;

EncryptedSocket::EncryptedSocket() {
	static int r = 0;
	if(!r) {
		Socket::setPoll(&socketPoll);
		new std::thread(&updatePoll);
		r = 1;
	}
}

EncryptedSocket::~EncryptedSocket() {
	abort();
}

bool EncryptedSocket::connectToHost(const std::string & hostName, quint16 port) {
    inputEnc.prepare("}h79q~B%al;k'y $E");
    outputEnc.prepare("}h79q~B%al;k'y $E");

	return Socket::connect(hostName.c_str(), port);
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

void EncryptedSocket::updatePoll() {
	socketPoll.run();
}
