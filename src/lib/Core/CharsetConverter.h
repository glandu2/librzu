#ifndef CHARSETCONVERTER_H
#define CHARSETCONVERTER_H

#include "Object.h"
#include "iconv.h"
#include <string>

class CharsetConverter : public Object
{
public:
	CharsetConverter(const char* from, const char* to);
	~CharsetConverter();

	//return number of significant bytes in converted buffer
	void convert(const std::string &in, std::string& out, float ratioHint);
	
protected:
	int iconvIgnoreInvalid(const char** inbuf, int* inSize, char** outbuf, int* outSize);

private:
	iconv_t ic;
};

#endif // CHARSETCONVERTER_H
