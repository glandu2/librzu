#ifndef ENCRYPTEDSOCKET_H
#define ENCRYPTEDSOCKET_H

#include "RC4Cipher.h"
#include <QTcpSocket>

class EncryptedSocket : public QTcpSocket
{
	public:
		EncryptedSocket();
		~EncryptedSocket();

		bool connectToHost(const QString & hostName, quint16 port, OpenMode openMode = ReadWrite);
		qint64 read(char *buffer, qint64 size);
		qint64 write(const char *buffer, qint64 size);
		inline qint64 read(void *buffer, qint64 size) { return read((char*)buffer, size); }
		inline qint64 write(const void *buffer, qint64 size) { return write((const char*)buffer, size); }

	private:
		RC4Cipher inputEnc, outputEnc;
};

#endif // ENCRYPTEDSOCKET_H
