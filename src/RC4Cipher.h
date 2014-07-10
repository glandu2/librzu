#ifndef RC4CIPHER_H
#define RC4CIPHER_H

#include "Object.h"
#include <string.h>

class RAPPELZLIB_EXTERN RC4Cipher : public Object
{
	DECLARE_CLASS(RC4Cipher)

	public:
		RC4Cipher();
		void prepare(const char *key);
		void encode(const char *in, char *out, size_t size);

		RC4Cipher& operator=(const RC4Cipher& other) {
			memcpy(&state, &other.state, sizeof(state));
			return *this;
		}

	private:
		void rappelz_modified_RC4_set_key(int len, const unsigned char *data);

		struct RC4Key
		{
			unsigned int x,y;
			unsigned int data[256];
		} state;
};

#endif // RC4CIPHER_H
