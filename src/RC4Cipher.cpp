#include "RC4Cipher.h"
#include <string.h>

inline void exchange(char *a, char *b) {
	char tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

RC4Cipher::RC4Cipher() {
	prepare("Neat & Simple");
}

void RC4Cipher::prepare(const char *key) {
	int i, j, keyLen = strlen(key);

	for(i=0; i<256; i++)
		s[i] = i;

	for(i=0,j=0; i<256; i++) {
		j = (j + s[j] + key[j%keyLen]) & 0xFF;
		exchange(&s[i], &s[j]);
	}

	//simulate transmition with 1013 bytes
	y=0;
	x=0;
	for(i=0; i<1013; i++) {
		x = (x + 1) & 0xFF;
		y = (s[x] + y) & 0xFF;
		exchange(&s[x], &s[y]);
	}
}

void RC4Cipher::getXOR(char *out, size_t size) {
	size_t i;

	for(i=0; i<size; i++) {
		x = (x + 1) & 0xFF;
		y = (s[x] + y) & 0xFF;
		exchange(&s[x], &s[y]);
		out[i] = s[(s[x] + s[y]) & 0xFF];
	}
}

void RC4Cipher::encode(const char *in, char *out, size_t size) {
	size_t i;

	for(i=0; i<size; i++) {
		x = (x + 1) & 0xFF;
		y = (s[x] + y) & 0xFF;
		exchange(&s[x], &s[y]);
		out[i] = in[i] ^ s[(s[x] + s[y]) & 0xFF];
	}
}
