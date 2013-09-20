#ifndef RSACIPHER_H
#define RSACIPHER_H

#include <QByteArray>

#include <openssl/rsa.h>

class RSACipher
{
	public:
		RSACipher(unsigned int keySize = 1024, unsigned int exponent = 65537);
		~RSACipher();

		QByteArray getPublicKey();
		QByteArray encrypt(QByteArray data);
		QByteArray decrypt(QByteArray data);

	private:
		RSA* rsaCipher;
};

#endif // RSACIPHER_H
