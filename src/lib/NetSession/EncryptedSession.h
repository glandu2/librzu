#ifndef ENCRYPTEDSESSION_H
#define ENCRYPTEDSESSION_H

#include "Cipher/RC4Cipher.h"
#include "Stream/Stream.h"
#include <stdint.h>

class Stream;

template<class SessionClass> class EncryptedSession : public SessionClass {
public:
	template<class... Args> EncryptedSession(Args... args) : SessionClass(args...) { initRC4Cipher(); }

	virtual ~EncryptedSession() {}

	virtual bool connect(const char* url, uint16_t port);

protected:
	void initRC4Cipher();

	size_t read(void* buffer, size_t size);
	size_t write(Stream::WriteRequest* writeRequest);
	size_t write(const void* buffer, size_t size);

private:
	RC4Cipher inputEnc, outputEnc;
};
extern RZU_EXTERN RC4Cipher cachedCipher;

template<class T> void EncryptedSession<T>::initRC4Cipher() {
	outputEnc = inputEnc = cachedCipher;
}

template<class T> bool EncryptedSession<T>::connect(const char* url, uint16_t port) {
	initRC4Cipher();

	return T::connect(url, port);
}

template<class T> size_t EncryptedSession<T>::read(void* buffer, size_t size) {
	size_t ret = T::read((char*) buffer, size);

	inputEnc.encode((const char*) buffer, (char*) buffer, ret);

	return ret;
}

template<class T> size_t EncryptedSession<T>::write(Stream::WriteRequest* writeRequest) {
	size_t ret;

	outputEnc.encode((const char*) writeRequest->buffer.base, writeRequest->buffer.base, writeRequest->buffer.len);
	ret = T::write(writeRequest);

	return ret;
}

template<class T> size_t EncryptedSession<T>::write(const void* buffer, size_t size) {
	size_t ret;
	Stream::WriteRequest* writeRequest = Stream::WriteRequest::create(size);

	outputEnc.encode((const char*) buffer, writeRequest->buffer.base, size);
	ret = T::getStream()->write(writeRequest);

	return ret;
}

#endif  // ENCRYPTEDSESSION_H
