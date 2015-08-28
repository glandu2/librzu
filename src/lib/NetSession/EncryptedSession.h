#ifndef ENCRYPTEDSESSION_H
#define ENCRYPTEDSESSION_H

#include "Cipher/RC4Cipher.h"
#include <stdint.h>
#include <stdlib.h>
#ifdef _WIN32
#include <malloc.h>
#else
#include <alloca.h>
#endif

class Stream;

template<class SessionClass>
class EncryptedSession : public SessionClass
{
public:

	EncryptedSession() {
		initRC4Cipher();
	}

	virtual ~EncryptedSession() {}

	virtual bool connect(const char* url, uint16_t port);

protected:
	void initRC4Cipher();

	size_t read(void *buffer, size_t size);
	size_t write(const void *buffer, size_t size);

private:
	RC4Cipher inputEnc, outputEnc;
};
extern RZU_EXTERN RC4Cipher cachedCipher;

template<class T>
void EncryptedSession<T>::initRC4Cipher() {
	outputEnc = inputEnc = cachedCipher;
}

template<class T>
bool EncryptedSession<T>::connect(const char* url, uint16_t port) {
	initRC4Cipher();

	return T::connect(url, port);
}

template<class T>
size_t EncryptedSession<T>::read(void *buffer, size_t size) {
	size_t ret = T::read((char*)buffer, size);

	inputEnc.encode((const char*)buffer, (char*)buffer, ret);

	return ret;
}

template<class T>
size_t EncryptedSession<T>::write(const void *buffer, size_t size) {
	size_t ret;
	char *encbuffer;
	bool useStackAlloc = size < 16*1024;

	if(useStackAlloc)
		encbuffer = (char*)alloca(size);
	else
		encbuffer = (char*)malloc(size);

	outputEnc.encode((const char*)buffer, encbuffer, size);
	ret = T::write(encbuffer, size);

	if(!useStackAlloc)
		free(encbuffer);

	return ret;
}

#endif // ENCRYPTEDSESSION_H
