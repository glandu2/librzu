#ifndef RZHASHREVERSIBLE256_H
#define RZHASHREVERSIBLE256_H

#include "../Extern.h"
#include <stdint.h>

struct TS_CS_VERSION;
struct TS_CS_LOGIN;

class RZU_EXTERN RzHashReversible256 {
private:
	/**
	 * @brief Generate a 256 bytes payload based on given input.
	 * @param dataOut 256 byte buffer that will contains the result.
	 * @param input input data. Must be at least 3 bytes long.
	 */
	static void encrypt(uint8_t* dataOut, const char* input);

public:
	static void generatePayload(TS_CS_VERSION& versionPkt);
	static void generatePayload(TS_CS_LOGIN& versionPkt);
};

#endif
