#ifndef UTILS_H
#define UTILS_H

#include "RappelzLib_global.h"
#include <time.h>
#include <string>
#include <stdio.h>

#if defined(_MSC_VER)
	#define snprintf(buffer, size, ...) _snprintf_s(buffer, size, _TRUNCATE, __VA_ARGS__)
	#if _MSC_VER < 1800
		#define va_copy(d,s) ((d) = (s))
	#endif
#endif

class IListener;
template<typename T> class cval;

class RAPPELZLIB_EXTERN Utils
{
public:
	static struct tm * getGmTime(time_t secs, struct tm *tm);
	static int mkdir(const char* dir);
	static const char* getApplicationPath();
	static std::string getFullPath(const std::string& partialPath);
	static bool isAbsolute(const char* dir);
	static void* memmem(const void *haystack, size_t hlen, const void *needle, size_t nlen);
	static void autoSetAbsoluteDir(cval<std::string>& value);

private:
	static void autoSetAbsoluteDirConfigValue(IListener*, cval<std::string>* value);
};

#endif // UTILS_H
