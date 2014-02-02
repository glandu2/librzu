#include "EncryptedSocket.h"
#include <stdio.h>
#include <malloc.h>

EncryptedSocket::EncryptedSocket(uv_loop_t* uvLoop, Mode mode) : Socket(uvLoop), useEncryption(mode == Encrypted) {
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

bool EncryptedSocket::connect(const std::string & hostName, uint16_t port) {
	if(useEncryption) {
		inputEnc.prepare("}h79q~B%al;k'y $E");
		outputEnc.prepare("}h79q~B%al;k'y $E");
	}

	return Socket::connect(hostName.c_str(), port);
}

size_t EncryptedSocket::read(void *buffer, size_t size) {
	size_t ret = Socket::read((char*)buffer, size);

	if(useEncryption)
		inputEnc.encode((const char*)buffer, (char*)buffer, ret);

    return ret;
}

size_t EncryptedSocket::write(const void *buffer, size_t size) {
    char *encbuffer = (char*)alloca(size);
	size_t ret;

	if(useEncryption) {
		outputEnc.encode((const char*)buffer, encbuffer, size);
		ret = Socket::write(encbuffer, size);
	} else {
		ret = Socket::write(buffer, size);
	}

    return ret;
}
