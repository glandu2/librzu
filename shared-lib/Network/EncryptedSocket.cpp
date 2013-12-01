#include "EncryptedSocket.h"
#include <stdio.h>
#include <malloc.h>

SocketPoll EncryptedSocket::socketPoll;

EncryptedSocket::EncryptedSocket() {
//	static int r = 0;
//	if(!r) {
//		Socket::setPoll(&socketPoll);
//		new std::thread(&updatePoll);
//		r = 1;
//	}
	inputEnc.prepare("}h79q~B%al;k'y $E");
	outputEnc.prepare("}h79q~B%al;k'y $E");
}

EncryptedSocket::~EncryptedSocket() {
	abort();
}

bool EncryptedSocket::connect(const std::string & hostName, uint64_t port) {
    inputEnc.prepare("}h79q~B%al;k'y $E");
    outputEnc.prepare("}h79q~B%al;k'y $E");

	return Socket::connect(hostName.c_str(), port);
}

size_t EncryptedSocket::read(void *buffer, size_t size) {
	int64_t ret = Socket::read((char*)buffer, size);
    inputEnc.encode((const char*)buffer, (char*)buffer, ret);

    return ret;
}

size_t EncryptedSocket::write(const void *buffer, size_t size) {
    char *encbuffer = (char*)alloca(size);
	int64_t ret;

    outputEnc.encode((const char*)buffer, encbuffer, size);
	ret = Socket::write(encbuffer, size);

    return ret;
}

void EncryptedSocket::updatePoll() {
	socketPoll.run();
}
