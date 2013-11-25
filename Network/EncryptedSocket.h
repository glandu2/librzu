#ifndef ENCRYPTEDSOCKET_H
#define ENCRYPTEDSOCKET_H

#include "RC4Cipher.h"
#include "Socket.h"
#include <QString>
#include <QAbstractSocket>
#include <QThread>
#include "SocketPoll.h"

class EncryptedSocket : public QObject, public Socket
{
	Q_OBJECT
	public:
		EncryptedSocket();
		~EncryptedSocket();

		bool connectToHost(const QString & hostName, quint16 port);
		qint64 read(char *buffer, qint64 size);
		qint64 write(const char *buffer, qint64 size);
		inline qint64 read(void *buffer, qint64 size) { return read((char*)buffer, size); }
		inline qint64 write(const void *buffer, qint64 size) { return write((const char*)buffer, size); }
		QAbstractSocket::SocketState state();

		bool waitForDisconnected(int i) { i=i; return true; }
		QString errorString() { return QString::number(errno); }
		quint64 bytesAvailable() { return getAvailableBytes(); }

	signals:
		void readyRead();
		void connected();
		void disconnected();
		void error(QAbstractSocket::SocketError);

	private:
		RC4Cipher inputEnc, outputEnc;

		static void onDataReady(void* instance, ISocket* socket);
		static void onStateChanged(void* instance, ISocket* socket, State oldState, State newState);
		static void onError(void* instance, ISocket* socket, int errnoValue);

		static SocketPoll socketPoll;

		static void updatePoll();

};

#endif // ENCRYPTEDSOCKET_H
