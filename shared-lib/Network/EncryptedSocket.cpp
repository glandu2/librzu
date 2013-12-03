#include "EncryptedSocket.h"
#include <stdio.h>
#include <malloc.h>

SocketPoll EncryptedSocket::socketPoll;

EncryptedSocket::EncryptedSocket(bool useEncryption) : useEncryption(useEncryption) {
//	static int r = 0;
//	if(!r) {
//		Socket::setPoll(&socketPoll);
//		new std::thread(&updatePoll);
//		r = 1;
//	}
	if(useEncryption) {
		inputEnc.prepare("}h79q~B%al;k'y $E");
		outputEnc.prepare("}h79q~B%al;k'y $E");
	}
}

EncryptedSocket::~EncryptedSocket() {
	abort();
}

bool EncryptedSocket::connect(const std::string & hostName, uint64_t port) {
	if(useEncryption) {
		inputEnc.prepare("}h79q~B%al;k'y $E");
		outputEnc.prepare("}h79q~B%al;k'y $E");
	}

	return Socket::connect(hostName.c_str(), port);
}

size_t EncryptedSocket::read(void *buffer, size_t size) {
	int64_t ret = Socket::read((char*)buffer, size);

	if(useEncryption)
		inputEnc.encode((const char*)buffer, (char*)buffer, ret);

    return ret;
}

size_t EncryptedSocket::write(const void *buffer, size_t size) {
    char *encbuffer = (char*)alloca(size);
	int64_t ret;

	if(useEncryption) {
		outputEnc.encode((const char*)buffer, encbuffer, size);
		ret = Socket::write(encbuffer, size);
	} else {
		ret = Socket::write(buffer, size);
	}

    return ret;
}

void EncryptedSocket::updatePoll() {
	socketPoll.run();
}
