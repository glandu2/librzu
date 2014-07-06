#include "RC4Cipher.h"
#include <string.h>

inline void exchange(RC4_INT *a, RC4_INT *b) {
	RC4_INT tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

RC4Cipher::RC4Cipher() {
	//prepare("Neat & Simple");
}

static void rappelz_modified_RC4_set_key(RC4_KEY *key, int len, const unsigned char *data)
{
	register RC4_INT tmp;
	register int j;
	register RC4_INT *d;
	unsigned int i;

	d= &(key->data[0]);
	key->x = 0;
	key->y = 0;
	j=0;

#define SK_LOOP(d,n) { \
	j = (data[j%len] + d[j] + j) & 0xff; \
	tmp=d[(n)]; \
	d[(n)]=d[j]; \
	d[j]=tmp; }

	for (i=0; i < 256; i++) d[i]=i;
	for (i=0; i < 256; i+=4)
	{
		SK_LOOP(d,i+0);
		SK_LOOP(d,i+1);
		SK_LOOP(d,i+2);
		SK_LOOP(d,i+3);
	}
}

void RC4Cipher::prepare(const char *key) {
	rappelz_modified_RC4_set_key(&state, strlen(key), (const unsigned char*)key);

	//simulate transmition with 1013 bytes
	state.y=0;
	state.x=0;
	unsigned int i;
	for(i=0; i<1013; i++) {
		state.x = (state.x + 1) & 0xFF;
		state.y = (state.data[state.x] + state.y) & 0xFF;
		exchange(&state.data[state.x], &state.data[state.y]);
	}
}

void RC4Cipher::encode(const char *in, char *out, size_t size) {
	RC4(&state, size, (const unsigned char*)in, (unsigned char*)out);
}
