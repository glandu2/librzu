#ifndef ENCRYPTEDSOCKET_H
#define ENCRYPTEDSOCKET_H

#include "RC4Cipher.h"

class Stream;

template<class SessionClass>
class EncryptedSession : public SessionClass
{
public:

	EncryptedSession() {
		initRC4Cipher();
	}

	virtual ~EncryptedSession() {}

	virtual bool connect(const char* url);

protected:
	void initRC4Cipher();

	virtual size_t read(void *buffer, size_t size);
	virtual size_t write(const void *buffer, size_t size);

private:
	RC4Cipher inputEnc, outputEnc;
};
extern RAPPELZLIB_EXTERN RC4Cipher cachedCipher;

template<class T>
void EncryptedSession<T>::initRC4Cipher() {
	outputEnc = inputEnc = cachedCipher;
}

template<class T>
bool EncryptedSession<T>::connect(const char* url) {
	initRC4Cipher();

	return T::connect(url);
}

template<class T>
size_t EncryptedSession<T>::read(void *buffer, size_t size) {
	size_t ret = T::read((char*)buffer, size);

	inputEnc.encode((const char*)buffer, (char*)buffer, ret);

	return ret;
}

template<class T>
size_t EncryptedSession<T>::write(const void *buffer, size_t size) {
	char *encbuffer = (char*)alloca(size);
	size_t ret;

	outputEnc.encode((const char*)buffer, encbuffer, size);
	ret = T::write(encbuffer, size);

	return ret;
}

#endif // ENCRYPTEDSOCKET_H
