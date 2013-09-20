#include "RSACipher.h"

#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include <stdlib.h>

RSACipher::RSACipher(unsigned int keySize, unsigned int exponent) {
	rsaCipher = RSA_generate_key(keySize, exponent, NULL, NULL);
}

RSACipher::~RSACipher() {
	RSA_free(rsaCipher);
}

QByteArray RSACipher::getPublicKey() {
	char* publicKey;
	BIO *bio;

	bio = BIO_new(BIO_s_mem());

	PEM_write_bio_RSA_PUBKEY(bio, rsaCipher);

	int keyLen = BIO_pending(bio);
	publicKey = (char*)calloc(keyLen+1, 1); /* Null-terminate */
	BIO_read(bio, publicKey, keyLen);
	BIO_free(bio);

	QByteArray keyData(publicKey);
	free(publicKey);

	return keyData;
}

QByteArray RSACipher::encrypt(QByteArray data) {
	int dataLength;
	QByteArray ret;

	ret.resize(data.size());
	dataLength = RSA_public_encrypt(data.size(), (const unsigned char*)data.constData(), (unsigned char*)ret.data(), rsaCipher, RSA_PKCS1_PADDING);

	ret.resize(dataLength);
	return ret;
}

QByteArray RSACipher::decrypt(QByteArray data) {
	int dataLength;
	QByteArray ret;

	ret.resize(data.size());
	dataLength = RSA_private_decrypt(data.size(), (const unsigned char*)data.constData(), (unsigned char*)ret.data(), rsaCipher, RSA_PKCS1_PADDING);

	ret.resize(dataLength);
	return ret;
}
