#include "CharsetConverter.h"
#include "Config/GlobalCoreConfig.h"
#include "iconv.h"
#include <errno.h>
#include <memory>
#include <stdio.h>

IconvInstance::IconvInstance(const char* from, const char* to, void* ic) : from(from), to(to), ic(ic, &iconv_close) {
	uv_mutex_init(&mutex);
}
IconvInstance::~IconvInstance() {
	uv_mutex_destroy(&mutex);
}

int IconvInstance::convertIgnoreInvalid(const char** inBuf, int* inSize, char** outBuf, int* outSize) {
	size_t sInSize = *inSize;
	size_t sOutSize = *outSize;
	const char* inPtr = *inBuf;
	char* outPtr = *outBuf;
	size_t result;

	uv_mutex_lock(&mutex);

	iconv(ic.get(), nullptr, nullptr, nullptr, nullptr);
	while((result = iconv(ic.get(), &inPtr, &sInSize, &outPtr, &sOutSize)) == (size_t) -1 && sInSize > 1 &&
	      sOutSize > 0) {
		if(errno == EINVAL || errno == E2BIG)
			break;

		inPtr++;
		sInSize--;
	}
	uv_mutex_unlock(&mutex);

	*inBuf = inPtr;
	*inSize = (int) sInSize;
	*outBuf = outPtr;
	*outSize = (int) sOutSize;

	if(sInSize == 0)
		return 0;
	else {
		if(errno == EINVAL || errno == E2BIG)
			return -errno;
		else
			return -EINVAL;
	}
}

CharsetConverterGlobalCache::CharsetConverterGlobalCache() {
	uv_mutex_init(&mutex);
}

CharsetConverterGlobalCache::~CharsetConverterGlobalCache() {
	uv_mutex_destroy(&mutex);
}

IconvInstance* CharsetConverterGlobalCache::get(const char* from, const char* to) {
	uv_mutex_lock(&mutex);
	for(IconvInstance& ic : iconvInstances) {
		if(ic.from == from && ic.to == to) {
			uv_mutex_unlock(&mutex);
			return &ic;
		}
	}

	iconv_t ic = iconv_open(to, from);
	if(!ic || ic == (iconv_t) -1) {
		log(LL_Error, "Cant open iconv: from: \"%s\", to: \"%s\"\n", from, to);

		uv_mutex_unlock(&mutex);
		return nullptr;
	} else {
		iconvInstances.emplace_back(from, to, ic);
		IconvInstance* instance = &iconvInstances.back();

		uv_mutex_unlock(&mutex);
		return instance;
	}
}

CharsetConverterGlobalCache CharsetConverter::cache;

CharsetConverter::CharsetConverter(const char* from, const char* to) {
	ic = cache.get(from, to);
}

CharsetConverter::~CharsetConverter() {}

void CharsetConverter::convert(const std::string& in, std::string& out, float ratioHint) {
	if(in.empty()) {
		out.clear();
		return;
	}

	if(!ic) {
		out = in;
		return;
	}

	out.resize(size_t(in.size() * ratioHint) + 1);

	const char* inPtr = in.data();
	char* outPtr = &out[0];
	int inRemainingBytes = (int) in.size();
	int outRemainingBytes = (int) out.size();

	while(inRemainingBytes > 0) {
		int result = ic->convertIgnoreInvalid(&inPtr, &inRemainingBytes, &outPtr, &outRemainingBytes);
		if(result < 0) {
			if(result == -E2BIG) {
				int outDone = (int) out.size() - outRemainingBytes;
				out.resize(out.size() * 2);
				outPtr = &out[outDone];
				outRemainingBytes = (int) out.size() - outDone;
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
