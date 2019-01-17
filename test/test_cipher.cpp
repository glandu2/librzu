#include "Cipher/ZlibCipher.h"
#include <iostream>

int main() {
	std::string randomString;
	std::vector<uint8_t> encryptedString;
	std::string decryptedString;

	for(size_t i = 0; i < 10000; i++) {
		randomString.resize(rand() % 40 + 10);
		for(size_t i = 0; i < randomString.size(); i++) {
			randomString[i] = (rand() % 255) + 1;
		}

		encryptedString = ZlibCipher::encrypt(randomString);
		decryptedString = ZlibCipher::decrypt(encryptedString);

		if(decryptedString != randomString) {
			printf("Failed to find original string, got \"%s\", expected \"%s\"\n",
			       decryptedString.c_str(),
			       randomString.c_str());
			return 1;
		}
	}

	return 0;
}
