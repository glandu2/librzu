#ifndef RC4CIPHER_H
#define RC4CIPHER_H

#include "Object.h"

class RAPPELZLIB_EXTERN RC4Cipher : public Object
{
	DECLARE_CLASS(RC4Cipher)

	public:
		RC4Cipher();
		void prepare(const char *key);
		void getXOR(char *out, size_t size);
		void encode(const char *in, char *out, size_t size);

	private:
		char s[256];
		unsigned char x, y;
};

#endif // RC4CIPHER_H
