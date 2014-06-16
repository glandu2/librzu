#ifndef ENCRYPTEDSOCKET_H
#define ENCRYPTEDSOCKET_H

#include "RC4Cipher.h"
#include "Socket.h"

class RAPPELZLIB_EXTERN EncryptedSocket : public Socket
{
	DECLARE_CLASS(EncryptedSocket)

	public:
		enum Mode {
				 NoEncryption,
				 Encrypted
};

	EncryptedSocket(uv_loop_t *uvLoop, Mode mode = Encrypted, bool logPackets = false);
	~EncryptedSocket();

	virtual bool connect(const std::string &hostName, uint16_t port);
	virtual size_t read(void *buffer, size_t size);
	virtual size_t write(const void *buffer, size_t size);

protected:
	void initRC4Cipher();

private:
	RC4Cipher inputEnc, outputEnc;
	bool useEncryption;

	static RC4Cipher cachedCipher;
	static std::string cachedCipherKey;

};

#endif // ENCRYPTEDSOCKET_H
