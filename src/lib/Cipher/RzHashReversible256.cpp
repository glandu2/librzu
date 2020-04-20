#include "RzHashReversible256.h"
#include <algorithm>
#include <stdlib.h>
#include <string.h>  // for memcpy
#include <string>    // for string

#include "GameClient/TS_CS_LOGIN.h"
#include "GameClient/TS_CS_VERSION.h"

void RzHashReversible256::encrypt(uint8_t* dataOut, const char* input) {
	uint16_t random1 = (rand() & 0x7FFE) + 1;
	uint16_t random1Reduced = random1 >> 4;
	uint16_t acc = 0;
	const uint8_t* uInput = reinterpret_cast<const uint8_t*>(input);

	for(int i = 0; i < 256; i++) {
		switch(i) {
			case 221:
				dataOut[221] = random1 & 0xFF;
				break;
			case 245:
				dataOut[245] = random1 >> 8;
				break;
			case 136:
				dataOut[136] = random1Reduced + ~uInput[0];
				acc += 11 * uInput[0];
				break;
			case 33:
				dataOut[33] = random1Reduced + ~uInput[1];
				acc += 11 * uInput[1];
				break;
			case 126:
				dataOut[126] = random1Reduced + ~uInput[2];
				acc += 11 * uInput[2];
				break;
			default: {
				int random2 = rand() % 255;
				dataOut[i] = random1Reduced + ~random2;
				if((i % 7) == 0)
					acc += 2 * random2;
				else if((i % 4) == 1)
					acc -= 255 - random2;
			}
		}
	}
	dataOut[27] = random1Reduced + ~(acc >> 8);
	dataOut[99] = random1Reduced + ~(acc & 0xFF);
}

void RzHashReversible256::generatePayload(TS_CS_VERSION& versionPkt) {
	char buffer[3] = {0};
	int encryptableSize = (int) versionPkt.szVersion.size() - 5;
	if(encryptableSize > 0)
		memcpy(buffer, versionPkt.szVersion.c_str() + 5, std::min(encryptableSize, 3));
	RzHashReversible256::encrypt(versionPkt.checksum, buffer);
}

void RzHashReversible256::generatePayload(TS_CS_LOGIN& loginPkt) {
	char buffer[3] = {0};
	memcpy(buffer, loginPkt.name.c_str(), std::min(loginPkt.name.size(), (size_t) 3));
	RzHashReversible256::encrypt(loginPkt.checksum, buffer);
}
