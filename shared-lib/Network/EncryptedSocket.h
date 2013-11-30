#ifndef ENCRYPTEDSOCKET_H
#define ENCRYPTEDSOCKET_H

#include "RC4Cipher.h"
#include "Socket.h"
#include "SocketPoll.h"

class EncryptedSocket : public Socket
{
	public:
		EncryptedSocket();
		~EncryptedSocket();

		virtual bool connect(const std::string &hostName, uint64_t port);
		size_t IFACECALLCONV read(void *buffer, size_t size);
		size_t IFACECALLCONV write(const void *buffer, size_t size);

		bool waitForDisconnected(int i) { i=i; return true; }

	private:
		RC4Cipher inputEnc, outputEnc;

		static SocketPoll socketPoll;

		static void updatePoll();

};

#endif // ENCRYPTEDSOCKET_H
