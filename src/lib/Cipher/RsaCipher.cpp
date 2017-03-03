#include "RsaCipher.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <string.h>

RsaCipher::RsaCipher()
    : rsaCipher(nullptr, &RSA_free)
{
}

RsaCipher::~RsaCipher() {
}

void RsaCipher::loadKey(const std::vector<uint8_t>& pemKey) {
	std::unique_ptr<BIO, int(*)(BIO*)> bio(nullptr, &BIO_free);

	bio.reset(BIO_new_mem_buf((void*)pemKey.data(), (int)pemKey.size()));
	rsaCipher.reset(PEM_read_bio_RSA_PUBKEY(bio.get(), NULL, NULL, NULL));
}

void RsaCipher::getPemPublicKey(std::vector<uint8_t>& outKey) {
	if(rsaCipher) {
		std::unique_ptr<BIO, decltype(&::BIO_free)> b (BIO_new(BIO_s_mem()), ::BIO_free);
		PEM_write_bio_RSA_PUBKEY(b.get(), rsaCipher.get());

		outKey.resize(BIO_get_mem_data(b.get(), nullptr));
		BIO_read(b.get(), &outKey[0], (int)outKey.size());
	}
}

void RsaCipher::generateKey() {
	std::unique_ptr<BIGNUM, decltype(&::BN_free)> e (BN_new(), ::BN_free);

	rsaCipher.reset(RSA_new());
	BN_set_word(e.get(), RSA_F4);
	RSA_generate_key_ex(rsaCipher.get(), 1024, e.get(), NULL);
}

bool RsaCipher::publicEncrypt(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
	output.resize(RSA_size(rsaCipher.get()));
	int result = RSA_public_encrypt(input.size(), input.data(), output.data(), rsaCipher.get(), RSA_PKCS1_PADDING);
	if(result < 0) {
		output.clear();
		return false;
	}

	output.resize(result);
	return true;
}

bool RsaCipher::publicDecrypt(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
	output.resize(RSA_size(rsaCipher.get()));
	int result = RSA_public_decrypt(input.size(), input.data(), output.data(), rsaCipher.get(), RSA_PKCS1_PADDING);
	if(result < 0) {
		output.clear();
		return false;
	}

	output.resize(result);
	return true;
}

bool RsaCipher::privateEncrypt(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
	output.resize(RSA_size(rsaCipher.get()));
	int result = RSA_private_encrypt(input.size(), input.data(), output.data(), rsaCipher.get(), RSA_PKCS1_PADDING);
	if(result < 0) {
		output.clear();
		return false;
	}

	output.resize(result);
	return true;
}

bool RsaCipher::privateDecrypt(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
	output.resize(RSA_size(rsaCipher.get()));
	int result = RSA_private_decrypt(input.size(), input.data(), output.data(), rsaCipher.get(), RSA_PKCS1_PADDING);
	if(result < 0) {
		output.clear();
		return false;
	}

	output.resize(result);
	return true;
}
