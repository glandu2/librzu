#include "ZlibCipher.h"
#include <stdint.h>
#include "zlib.h"

int32_t ZlibCipher::getMagic(uint32_t checksum, uint32_t compressedSize) {
	int32_t magic = (((checksum & 0xFFF0) + 1) % compressedSize) / 2 - 2;
	if(magic < 2) {
		if(compressedSize > 16)
			magic = compressedSize / 2 - 1;
		else if(compressedSize > 4)
			magic = compressedSize - 2;
		else
			magic = compressedSize - 1;
	}

	std::vector<int> divisors;
	for(unsigned int i = 2; i < compressedSize; i++) {
		if((compressedSize % i) == 0)
			divisors.push_back(i);
	}

	while(magic > 1) {
		std::vector<int>::iterator it;
		for(it = divisors.begin(); it != divisors.end(); ++it) {
			if((magic % *it) == 0)
				break;
		}
		if(it == divisors.end())
			break;
		magic--;
	}

	return magic;
}

std::string ZlibCipher::decrypt(std::vector<unsigned char> encryptedData) {
	if(encryptedData.size() < 8)
		return std::string();

	uint32_t compressedSize = (uint32_t)(encryptedData.size() - 8);
	uint32_t checksum = encryptedData[compressedSize + 4] << 24 |
				encryptedData[compressedSize + 5] << 16 |
				encryptedData[compressedSize + 6] << 8 |
				encryptedData[compressedSize + 7];

	for(uint32_t i = 0; i < compressedSize + 4; i++) {
		encryptedData[i] ^= checksum;
	}

	uint32_t originalSize = encryptedData[compressedSize] << 24 |
					encryptedData[compressedSize + 1] << 16 |
					encryptedData[compressedSize + 2] << 8 |
					encryptedData[compressedSize + 3];

	int32_t magic = getMagic(checksum, compressedSize);

	unsigned char lastChar = 0;
	uint32_t idx = checksum % compressedSize;
	std::vector<unsigned char> compressed;

	compressed.resize(compressedSize);
	for(unsigned int i = 0; i < compressedSize; i++) {
		compressed[i] = encryptedData[idx] + lastChar;
		idx = (idx + magic) % compressedSize;
		lastChar = compressed[i];
	}

	unsigned long uncompressedSize = originalSize;
	std::string decrypted;
	decrypted.resize(uncompressedSize);
	int result = uncompress((Bytef*)&decrypted[0], &uncompressedSize, &compressed[0], (uLong)compressed.size());
	if(result != Z_OK)
		return std::string();

	decrypted.resize(uncompressedSize);
	return decrypted;
}
