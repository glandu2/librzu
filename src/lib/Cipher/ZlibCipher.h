#ifndef ZLIBCIPHER_H
#define ZLIBCIPHER_H

#include "../Extern.h"
#include <stdint.h>
#include <string>
#include <vector>

class RZU_EXTERN ZlibCipher {
public:
	static std::string decrypt(std::vector<uint8_t> encryptedData);
	static std::vector<uint8_t> encrypt(std::string plainData);

protected:
	static int32_t getMagic(uint32_t checksum, uint32_t compressedSize);
};

#endif  // ZLIBCIPHER_H
