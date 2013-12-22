#ifndef ENCRYPTEDSOCKET_H
#define ENCRYPTEDSOCKET_H

#include "RC4Cipher.h"
#include "Socket.h"

class RAPPELZLIB_EXTERN EncryptedSocket : public Socket
{
	DECLARE_CLASS(EncryptedSocket)

	public:
		EncryptedSocket(uv_loop_t *uvLoop, bool useEncryption = true);
		~EncryptedSocket();

		virtual bool connect(const std::string &hostName, uint64_t port);
		virtual size_t read(void *buffer, size_t size);
		virtual size_t write(const void *buffer, size_t size);

	private:
		RC4Cipher inputEnc, outputEnc;
		bool useEncryption;

};

#endif // ENCRYPTEDSOCKET_H
