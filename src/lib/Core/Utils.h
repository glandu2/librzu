#ifndef UTILS_H
#define UTILS_H

#include "../Extern.h"
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <time.h>
#include <vector>

#if defined(_MSC_VER)
#define snprintf(buffer, size, ...) _snprintf_s(buffer, size, _TRUNCATE, __VA_ARGS__)
#if _MSC_VER < 1800
#define va_copy(d, s) ((d) = (s))
#endif
#endif

#define Utils_countOf(a) (sizeof(a) / sizeof(*a))

class IListener;
template<typename T> class cval;

class RZU_EXTERN Utils {
public:
	static uint64_t getTimeInMsec();
	static struct tm* getGmTime(time_t secs, struct tm* tm);
	static int mkdir(const char* dir);
	static const char* getApplicationPath();
	static const char* getApplicationName();
	static std::string getFullPath(const std::string& partialPath);
	static bool isAbsolute(const char* dir);
	static void* memmem(const void* haystack, size_t hlen, const void* needle, size_t nlen);
	static void autoSetAbsoluteDir(cval<std::string>& value);
	static std::string convertToString(int i);
	static std::string convertToString(float i);
	static std::string convertToString(const char* str, int maxSize);

	static std::vector<unsigned char> convertToDataArray(const unsigned char* data, int maxSize, int usedSize);
	static std::vector<unsigned char> convertToDataArray(const unsigned char* data, int size);

	static void convertDataToHex(const void* data, int size, char* outHex);
	static std::vector<unsigned char> convertHexToData(const std::string& hex);

	static unsigned long getPid();

	static void stringFormatv(std::string& dest, const char* message, va_list args);
	static void stringFormat(std::string& dest, const char* message, ...) PRINTFCHECK(2, 3);
	static bool stringReplace(std::string& str, const std::string& from, const std::string& to);
	static void stringReplaceAll(std::string& str, const std::string& from, const std::string& to);
	static bool stringWildcardMatch(const char* input, const char* pWildText);

	static bool isalpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
	static bool isdigit(char c) { return c >= '0' && c <= '9'; }
	static bool isspace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'; }

private:
	static void autoSetAbsoluteDirConfigValue(IListener*, cval<std::string>* value);
	static void getApplicationFilePath();

	static char applicationPath[260];
	static char applicationName[260];
	static bool applicationFilePathInitialized;
};

#endif  // UTILS_H
