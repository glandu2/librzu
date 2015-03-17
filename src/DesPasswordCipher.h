#ifndef DESPASSWORDCIPHER_H
#define DESPASSWORDCIPHER_H

#include <stdlib.h>
#include <string.h>
#include "Extern.h"

class RZU_EXTERN DesPasswordCipher
{
	public:
		DesPasswordCipher(const char *password = NULL) { if(password) init(password); }

		void init(const char *password);
		bool encrypt(void *buf, int len);
		bool decrypt(void *buf, int len);

	private:
		char keySchedule[256];
};

#endif // DESPASSWORDCIPHER_H
