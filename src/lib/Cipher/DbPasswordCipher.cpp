#include "DbPasswordCipher.h"
#include "DesPasswordCipher.h"
#include "ZlibCipher.h"
#include <string.h>

std::string DbPasswordCipher::decrypt(std::vector<unsigned char> cryptedPassword) {
	const unsigned char zlibPasswordHeader[] = {0x45, 0x56, 0x00, 0x00, 0x03, 0x00};

	// Check if ZLIB based password beginning with the header, else use DES decryption
	if(cryptedPassword.size() > sizeof(zlibPasswordHeader) &&
	   memcmp(&cryptedPassword[0], zlibPasswordHeader, sizeof(zlibPasswordHeader)) == 0) {
		return ZlibCipher::decrypt(
		    std::vector<unsigned char>(cryptedPassword.begin() + sizeof(zlibPasswordHeader), cryptedPassword.end()));
	} else {
		DesPasswordCipher("!_a^Rc*|#][Ych$~'(M _!d4aUo^%${T!~}h*&X%")
		    .decrypt(&cryptedPassword[0], (int) cryptedPassword.size());
		return std::string(cryptedPassword.begin(), cryptedPassword.end());
	}
}
