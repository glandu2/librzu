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

		bool connectToHost(const std::string &hostName, quint16 port);
		qint64 read(char *buffer, qint64 size);
		qint64 write(const char *buffer, qint64 size);
		inline qint64 read(void *buffer, qint64 size) { return read((char*)buffer, size); }
		inline qint64 write(const void *buffer, qint64 size) { return write((const char*)buffer, size); }

		bool waitForDisconnected(int i) { i=i; return true; }
		quint64 bytesAvailable() { return getAvailableBytes(); }

	private:
		RC4Cipher inputEnc, outputEnc;

		static SocketPoll socketPoll;

		static void updatePoll();

};

#endif // ENCRYPTEDSOCKET_H
