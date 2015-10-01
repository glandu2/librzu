#include "CharsetConverter.h"
#include <errno.h>
#include <stdio.h>
#include "Log.h"
#include "Config/GlobalCoreConfig.h"
#include "iconv.h"

CharsetConverter::CharsetConverter(const char *from, const char *to) {
	ic = iconv_open(to, from);
	if(!ic || ic == (iconv_t)-1) {
		log(LL_Error, "Cant open iconv: from: \"%s\", to: \"%s\"\n", from, to);
		ic = nullptr;
	}
}

CharsetConverter::~CharsetConverter() {
	if(ic)
		iconv_close(ic);
}

void CharsetConverter::convert(const std::string& in, std::string& out, float ratioHint) {
	if(in.empty()) {
		out.clear();
		return;
	}

	out.resize(size_t(in.size() * ratioHint) + 1);

	const char* inPtr = in.data();
	char* outPtr = &out[0];
	int inRemainingBytes = (int)in.size();
	int outRemainingBytes = (int)out.size();

	while(inRemainingBytes > 0) {
		int result = iconvIgnoreInvalid(&inPtr, &inRemainingBytes, &outPtr, &outRemainingBytes);
		if(result < 0) {
			if(result == -E2BIG) {
				int outDone = (int)out.size() - outRemainingBytes;
				out.resize(out.size() * 2);
				outPtr = &out[outDone];
				outRemainingBytes = (int)out.size() - outDone;
			} else {
				break;
			}
		}
	}

	out.resize(out.size() - outRemainingBytes);
}

std::string CharsetConverter::getEncoding() {
	return GlobalCoreConfig::get()->app.encoding.get();
}

int CharsetConverter::iconvIgnoreInvalid(const char** inBuf, int* inSize, char** outBuf, int* outSize) {
	size_t sInSize = *inSize;
	size_t sOutSize = *outSize;
	const char* inPtr = *inBuf;
	char* outPtr = *outBuf;
	size_t result;

	iconv(ic, nullptr, nullptr, nullptr, nullptr);
	while((result = iconv(ic, &inPtr, &sInSize, &outPtr, &sOutSize)) == (size_t)-1 && sInSize > 1 && sOutSize > 0) {
		if(errno == EINVAL || errno == E2BIG)
			break;

		inPtr++;
		sInSize--;
	}

	*inBuf = inPtr;
	*inSize = (int)sInSize;
	*outBuf = outPtr;
	*outSize = (int)sOutSize;

	if(sInSize == 0)
		return 0;
	else {
		if(errno == EINVAL || errno == E2BIG)
			return -errno;
		else
			return -EINVAL;
	}
}
