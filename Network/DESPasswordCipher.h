#ifndef DESPASSWORDCIPHER_H
#define DESPASSWORDCIPHER_H
#include <stdlib.h>

class DESPasswordCipher
{
	public:
		DESPasswordCipher(const char *password = NULL) { if(password) init(password); }

		virtual void init(const char *password);
		virtual bool encrypt(void *buf, int len);
		virtual bool decrypt(void *buf, int len);

	private:

		// End of DES algorithm (except for calling desinit below)
		void DesMem( void *buf, int mlen, int isencrypting );

		// 32-bit permutation at end
		void perm32(char *inblock, char *outblock);

		// contract f from 48 to 32 bits
		void contract(char *in48, char *out32);

		// critical cryptographic trans
		void f(char *right, int num, char *fret);

		// 1 churning operation
		void iter(int num, char *inblock, char *outblock);

		// initialize s1-s8 arrays
		void sinit(void);

		// initialize key schedule array
		void kinit(char *key64bit);

		// initialize 32-bit permutation
		void p32init(void);

		// encrypt 64-bit inblock
		void endes(char *inblock, char *outblock);

		// decrypt 64-bit inblock
		void dedes(char *inblock, char *outblock);

		// inital and final permutations
		char m_iperm[16][16][8];
		char m_fperm[16][16][8];

		char m_s[4][4096];				// S1 thru S8 precomputed
		char m_p32[4][256][4];			// for permuting 32-bit f output
		char m_kn[16][6];				// key selections

};

#endif // DESPASSWORDCIPHER_H
