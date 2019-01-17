#ifndef DBPASSWORDCIPHER_H
#define DBPASSWORDCIPHER_H

#include "../Extern.h"
#include <string>
#include <vector>

class RZU_EXTERN DbPasswordCipher {
public:
	static std::string decrypt(std::vector<unsigned char> cryptedPassword);
	static std::vector<uint8_t> encrypt(std::string password);
};

#endif  // DBPASSWORDCIPHER_H
