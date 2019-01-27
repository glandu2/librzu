#include "Cipher/ZlibCipher.h"
#include "gtest/gtest.h"

TEST(ZlibCipher, simple_loop_random) {
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

		ASSERT_STREQ(randomString.c_str(), decryptedString.c_str());
	}
}
