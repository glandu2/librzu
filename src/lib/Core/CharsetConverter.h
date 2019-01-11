#ifndef CHARSETCONVERTER_H
#define CHARSETCONVERTER_H

#include "Object.h"
#include "uv.h"
#include <deque>
#include <memory>
#include <string>

struct RZU_EXTERN IconvInstance {
	IconvInstance(const char* from, const char* to, void* ic);
	~IconvInstance();

	int convertIgnoreInvalid(const char** inbuf, int* inSize, char** outbuf, int* outSize);

	std::string from;
	std::string to;
	std::unique_ptr<void, int (*)(void*)> ic;
	mutable uv_mutex_t mutex;
};

class RZU_EXTERN CharsetConverterGlobalCache : public Object {
public:
	CharsetConverterGlobalCache();
	~CharsetConverterGlobalCache();

	IconvInstance* get(const char* from, const char* to);

	CharsetConverterGlobalCache(const CharsetConverterGlobalCache&) = delete;
	CharsetConverterGlobalCache& operator=(const CharsetConverterGlobalCache&) = delete;

private:
	std::deque<IconvInstance> iconvInstances;
	uv_mutex_t mutex;
};

class RZU_EXTERN CharsetConverter {
private:
public:
	CharsetConverter(const char* from, const char* to);
	~CharsetConverter();

	// return number of significant bytes in converted buffer
	void convert(const std::string& in, std::string& out, float ratioHint);

	static std::string getEncoding();

private:
	static CharsetConverterGlobalCache cache;
	IconvInstance* ic;
};

#endif  // CHARSETCONVERTER_H
