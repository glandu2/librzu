#ifndef DBPASSWORDCIPHER_H
#define DBPASSWORDCIPHER_H

#include "../Extern.h"
#include <vector>
#include <string>

class RZU_EXTERN DbPasswordCipher
{
public:
	static std::string decrypt(std::vector<unsigned char> cryptedPassword);
};

#endif // DBPASSWORDCIPHER_H
