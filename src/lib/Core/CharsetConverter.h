#ifndef CHARSETCONVERTER_H
#define CHARSETCONVERTER_H

#include "Object.h"
#include <string>

class RZU_EXTERN CharsetConverter : public Object
{
public:
	CharsetConverter(const char* from, const char* to);
	~CharsetConverter();

	//return number of significant bytes in converted buffer
	void convert(const std::string &in, std::string& out, float ratioHint);

	static std::string getEncoding();
	
protected:
	int iconvIgnoreInvalid(const char** inbuf, int* inSize, char** outbuf, int* outSize);

private:
	void* ic;
};

#endif // CHARSETCONVERTER_H
