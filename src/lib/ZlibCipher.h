#ifndef ZLIBCIPHER_H
#define ZLIBCIPHER_H

#include <string>
#include <vector>
#include "Extern.h"
#include <stdint.h>

class RZU_EXTERN ZlibCipher
{
public:
	static std::string decrypt(std::vector<unsigned char> encryptedData);

protected:
	static int32_t getMagic(uint32_t checksum, uint32_t compressedSize);
};

#endif // ZLIBCIPHER_H
