#ifndef RC4CIPHER_H
#define RC4CIPHER_H

#include "Object.h"

class RC4Cipher : public Object
{
	DECLARE_CLASS(RC4Cipher)

	public:
		RC4Cipher();
		void prepare(const char *key);
		void getXOR(char *out, int size);
		void encode(const char *in, char *out, int size);

	private:
		char s[256];
		int x, y;
};

#endif // RC4CIPHER_H
