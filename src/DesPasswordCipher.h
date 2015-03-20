#ifndef DESPASSWORDCIPHER_H
#define DESPASSWORDCIPHER_H

#include "Extern.h"

class RZU_EXTERN DesPasswordCipher
{
	public:
		DesPasswordCipher(const char *password = nullptr) { if(password) init(password); }

		void init(const char *password);
		bool encrypt(void *buf, int len);
		bool decrypt(void *buf, int len);

	private:
		char keySchedule[256];
};

#endif // DESPASSWORDCIPHER_H
