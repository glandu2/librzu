#ifndef RC4CIPHER_H
#define RC4CIPHER_H

#include "Object.h"
#include <string.h>

class LIB_EXTERN RC4Cipher : public Object
{
	DECLARE_CLASS(RC4Cipher)

	public:
		RC4Cipher(const char *key = nullptr) { if(key) prepare(key); }
		void prepare(const char *key);
		void encode(const char *in, char *out, size_t size);

		RC4Cipher& operator=(const RC4Cipher& other) {
			memcpy(&state, &other.state, sizeof(state));
			return *this;
		}

	private:
		void modified_RC4_set_key(int len, const unsigned char *data);

#ifdef __arm__
		struct RC4Key
		{
			unsigned char x,y;
			unsigned char data[256];
		};
#else
		struct RC4Key
		{
			unsigned int x,y;
			unsigned int data[256];
		};
#endif

		RC4Key state;
};

#endif // RC4CIPHER_H
