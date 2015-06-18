#include "Utils.h"
#include "ConfigParamVal.h"
#include <ctype.h>
#include <algorithm>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h> //for GetModuleFileName
#undef min
#undef max
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif


char Utils::applicationPath[260];
char Utils::applicationName[260];
bool Utils::applicationFilePathInitialized;


// From ffmpeg http://www.ffmpeg.org/doxygen/trunk/cutils_8c-source.html
#define ISLEAP(y) (((y) % 4 == 0) && (((y) % 100) != 0 || ((y) % 400) == 0))
#define LEAPS_COUNT(y) ((y)/4 - (y)/100 + (y)/400)

struct tm * Utils::getGmTime(time_t secs, struct tm *tm) {
	int days, y, ny, m;
	int md[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	days = (int)(secs / 86400);
	secs %= 86400;
	tm->tm_hour = (int)(secs / 3600);
	tm->tm_min = (secs % 3600) / 60;
	tm->tm_sec =  secs % 60;

	/* oh well, may be someone some day will invent a formula for this stuff */
	y = 1970; /* start "guessing" */
	while (days > 365) {
		ny = (y + days/366);
		days -= (ny - y) * 365 + LEAPS_COUNT(ny - 1) - LEAPS_COUNT(y - 1);
		y = ny;
	}
	if (days==365 && !ISLEAP(y)) { days=0; y++; }
	md[1] = ISLEAP(y)?29:28;
	for (m=0; days >= md[m]; m++)
		days -= md[m];

	tm->tm_year = y;  /* unlike gmtime_r we store complete year here */
	tm->tm_mon = m+1; /* unlike gmtime_r tm_mon is from 1 to 12 */
	tm->tm_mday = days+1;

	return tm;
}

int Utils::mkdir(const char* dir) {
#ifdef _WIN32
	if(::mkdir(dir) < 0)
		return errno;
	else
		return 0;
#else
	if(::mkdir(dir, 0755) < 0)
		return errno;
	else
		return 0;
#endif

}

void Utils::getApplicationFilePath() {
	if(applicationFilePathInitialized)
		return;

	char applicationFilePath[260];

	size_t n = sizeof(applicationFilePath);
	if(uv_exepath(applicationFilePath, &n) == 0)
		applicationFilePath[n] = '\0';
	else
		applicationFilePath[0] = '\0';

	if(applicationFilePath[0] == 0)
		strcpy(applicationFilePath, ".");

	//remove file name
	size_t len = strlen(applicationFilePath);
	char *p = applicationFilePath + len;
	while(p >= applicationFilePath) {
		if(*p == '/' || *p == '\\')
			break;
		p--;
	}
	if(p >= applicationFilePath)
		*p = 0;
	strcpy(applicationPath, applicationFilePath);
	if(p < applicationFilePath + len)
		strcpy(applicationName, p+1);
	else
		applicationName[0] = '\0';

	for(p = applicationName; *p != 0; p++) {
		if(*p == '.') {
			*p = '\0';
			break;
		}
	}

	applicationFilePathInitialized = true;
}

const char* Utils::getApplicationPath() {
	getApplicationFilePath();

	return applicationPath;
}

const char* Utils::getApplicationName() {
	getApplicationFilePath();

	return applicationName;
}

std::string Utils::getFullPath(const std::string &partialPath) {
	if(partialPath.size() >= 2 && partialPath.at(0) == '.' && (partialPath.at(1) == '/' || partialPath.at(1) == '\\'))
		return partialPath;

	if(isAbsolute(partialPath.c_str()))
		return partialPath;

	return std::string(Utils::getApplicationPath()) + '/' + partialPath;
}

bool Utils::isAbsolute(const char* dir) {
#ifdef _WIN32
	if(isalpha(dir[0]) && dir[1] == ':' && (dir[2] == '\\' || dir[2] == '/'))
		return true;
	else
		return false;
#else
	if(dir[0] == '/')
		return true;
	else
		return false;
#endif
}

void Utils::autoSetAbsoluteDir(cval<std::string>& value) {
	value.addListener(nullptr, &autoSetAbsoluteDirConfigValue);
	autoSetAbsoluteDirConfigValue(nullptr, &value);
}

std::string Utils::convertToString(int i) {
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%d", i);
	return std::string(buffer);
}

std::string Utils::convertToString(float i) {
	char buffer[128];
	snprintf(buffer, sizeof(buffer), "%f", i);
	return std::string(buffer);
}

std::string Utils::convertToString(const char *str, int maxSize) {
	return std::string(str, std::find(str, str + maxSize, '\0'));
}

std::vector<unsigned char> Utils::convertToDataArray(const unsigned char *data, int maxSize, int usedSize) {
	return std::vector<unsigned char>(data, data + std::max(0, std::min(maxSize, usedSize)));
}

std::vector<unsigned char> Utils::convertToDataArray(const unsigned char *data, int size) {
	return std::vector<unsigned char>(data, data + size);
}

void Utils::convertDataToHex(const void *data, int size, char *outHex) {
	static const char hexMapping[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	int i;
	for(i = 0; i < size; i++) {
		outHex[i*2] = hexMapping[static_cast<const char*>(data)[i] >> 4];
		outHex[i*2+1] = hexMapping[static_cast<const char*>(data)[i] & 0x0F];
	}
	outHex[i*2] = '\0';
}

std::vector<unsigned char> Utils::convertHexToData(const std::string &hex) {
	int i;
	int size = hex.size() / 2;
	std::vector<unsigned char> result;

	result.reserve(size);

	for(i = 0; i < size; i++) {
		unsigned char c = hex[i*2];
		unsigned char val = 0;

		if(c >= '0' && c <= '9')
			val = (c - '0') << 4;
		else if(c >= 'A' && c <= 'F')
			val = (c - 'A' + 10) << 4;
		else if(c >= 'a' && c <= 'f')
			val = (c - 'a' + 10) << 4;

		c = hex[i*2+1];

		if(c >= '0' && c <= '9')
			val |= (c - '0');
		else if(c >= 'A' && c <= 'F')
			val |= c - 'A' + 10;
		else if(c >= 'a' && c <= 'f')
			val |= c - 'a' + 10;

		result.push_back(val);
	}

	return result;
}

void Utils::autoSetAbsoluteDirConfigValue(IListener*, cval<std::string>* value) {
	std::string dir = value->get();
	std::string fullPath;

	if(isAbsolute(dir.c_str()))
		return;

	fullPath = getFullPath(dir);

	//keep defaultness
	if(value->setDefault(fullPath, false) == false)
		value->set(fullPath, false);
}

void* Utils::memmem(const void *haystack, size_t hlen, const void *needle, size_t nlen)
{
	int needle_first;
	const char *p = (const char*)haystack;
	size_t plen = hlen;

	if (!nlen)
		return NULL;

	needle_first = *(unsigned char *)needle;

	while (plen >= nlen && (p = (const char*)memchr(p, needle_first, plen - nlen + 1)))
	{
		if (!memcmp(p, needle, nlen))
			return (void *)p;

		p++;
		plen = hlen - (p - (const char*)haystack);
	}

	return NULL;
}
