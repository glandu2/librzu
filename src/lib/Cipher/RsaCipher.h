#ifndef RSACIPHER_H
#define RSACIPHER_H

#include "../Extern.h"
#include <vector>
#include <stdint.h>
#include <memory>

typedef struct rsa_st RSA;

class RZU_EXTERN RsaCipher
{
public:
	RsaCipher();
	~RsaCipher();

	void loadKey(const std::vector<uint8_t>& pemKey);
	void getPemPublicKey(std::vector<uint8_t>& outKey);
	void generateKey();

	bool publicEncrypt(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
	bool publicDecrypt(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
	bool privateEncrypt(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
	bool privateDecrypt(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);

private:
	std::unique_ptr<RSA, void (*)(RSA *)> rsaCipher;
};

#endif // RSACIPHER_H
