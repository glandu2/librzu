#include "EncryptedSocket.h"
#include <stdio.h>
#include <malloc.h>

RC4Cipher EncryptedSocket::cachedCipher;
std::string EncryptedSocket::cachedCipherKey;

EncryptedSocket::EncryptedSocket(uv_loop_t* uvLoop, Mode mode, bool logPackets) : Socket(uvLoop, logPackets), useEncryption(mode == Encrypted) {
	if(useEncryption) {
		initRC4Cipher();
	}
}

EncryptedSocket::~EncryptedSocket() {
	abort();
}

void EncryptedSocket::initRC4Cipher() {
	static const std::string cipherKey("}h79q~B%al;k'y $E");

	if(cachedCipherKey != cipherKey) {
		cachedCipher.prepare(cipherKey.c_str());
		cachedCipherKey = cipherKey;
	}

	outputEnc = inputEnc = cachedCipher;
}

bool EncryptedSocket::connect(const std::string & hostName, uint16_t port) {
	if(useEncryption) {
		initRC4Cipher();
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
