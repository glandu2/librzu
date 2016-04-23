#include "DbTypeHelpers.h"
#include "Core/Utils.h"

void DbDateTime::setUnixTime(time_t t, uint32_t nanoFraction)
{
	struct tm date;
	Utils::getGmTime(t, &date);

	year = date.tm_year;
	month = date.tm_mon;
	day = date.tm_mday;
	hour = date.tm_hour;
	minute = date.tm_min;
	second = date.tm_sec;
	fraction = nanoFraction;
}
