#ifndef AESPASSWORDCIPHER_H
#define AESPASSWORDCIPHER_H

#include "../Extern.h"
#include <vector>
#include <stdint.h>

typedef struct evp_cipher_ctx_st EVP_CIPHER_CTX;

class RZU_EXTERN AesPasswordCipher
{
public:
	AesPasswordCipher();
	~AesPasswordCipher();

	void init(const uint8_t *key);
	void init();
	void getKey(std::vector<uint8_t>& key);

	bool encrypt(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
	bool decrypt(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);

private:
	uint8_t key[32];
	std::unique_ptr<EVP_CIPHER_CTX, void(*)(EVP_CIPHER_CTX*)> evpCipher;
};

#endif // AESPASSWORDCIPHER_H
