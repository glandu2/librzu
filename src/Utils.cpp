#include "Utils.h"
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h> //for GetModuleFileName
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

// From ffmpeg http://www.ffmpeg.org/doxygen/trunk/cutils_8c-source.html
#define ISLEAP(y) (((y) % 4 == 0) && (((y) % 100) != 0 || ((y) % 400) == 0))
#define LEAPS_COUNT(y) ((y)/4 - (y)/100 + (y)/400)

struct tm * Utils::getGmTime(time_t secs, struct tm *tm) {
	int days, y, ny, m;
	int md[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	days = secs / 86400;
	secs %= 86400;
	tm->tm_hour = secs / 3600;
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

const char* Utils::getApplicationPath() {
	static char applicationPath[260];
	static bool initialized = false;

	if(initialized)
		return applicationPath;

#ifdef _WIN32
	GetModuleFileName(NULL, applicationPath, 259);
#else
	readlink("/proc/self/exe", applicationPath, 259);
#endif
	applicationPath[259] = 0;

	//remove file name
	char *p = applicationPath + strlen(applicationPath);
	while(p >= applicationPath) {
		if(*p == '/' || *p == '\\')
			break;
		p--;
	}
	if(p >= applicationPath)
		*p = 0;

	return applicationPath;
}

std::string Utils::getFullPath(const std::string &partialPath) {
	if(partialPath.size() >= 2 && partialPath.at(0) == '.' && (partialPath.at(1) == '/' || partialPath.at(1) == '\\'))
		return partialPath;

	return std::string(Utils::getApplicationPath()) + '/' + partialPath;
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
