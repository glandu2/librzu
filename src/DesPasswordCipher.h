#ifndef DESPASSWORDCIPHER_H
#define DESPASSWORDCIPHER_H

#include <stdlib.h>
#include <openssl/des.h>
#include <string.h>

class DesPasswordCipher
{
	public:
		DesPasswordCipher(const char *password = NULL) { if(password) init(password); }

		void init(const char *password) {
			unsigned char    key64bit[8];
			unsigned char   *key ;
			int     i;

			memset(key64bit, 0, sizeof(key64bit));

			key = key64bit ;
			for(i = 0; *password && i < 40; i++) {
				key[i % 8] ^= *password ;
				password++ ;
			}

			DES_set_odd_parity(&key64bit);
			DES_set_key_checked(&key64bit, &keySchedule);

		}

		bool encrypt(void *buf, int len) {
			len = len & 0xfffffff8;
			unsigned char desBlock[8];
			unsigned char *ptr = (unsigned char *)buf;
			for (; len > 0; len -= 8, ptr += 8) {                // encrypt/decrypt 1 buffer-full 8-byte blocks
				memcpy(desBlock, ptr, 8);
				DES_ecb_encrypt(&desBlock, &desBlock, &keySchedule, DES_ENCRYPT);
				memcpy(ptr, desBlock, 8);
			}
			return true;
		}

		bool decrypt(void *buf, int len) {
			len = len & 0xfffffff8;
			unsigned char desBlock[8];
			unsigned char *ptr = (unsigned char *)buf;
			for (; len > 0; len -= 8, ptr += 8) {                // encrypt/decrypt 1 buffer-full 8-byte blocks
				memcpy(desBlock, ptr, 8);
				DES_ecb_encrypt(&desBlock, &desBlock, &keySchedule, DES_DECRYPT);
				memcpy(ptr, desBlock, 8);
			}
			return true;
		}


	private:
		DES_key_schedule keySchedule;
};

#endif // DESPASSWORDCIPHER_H
