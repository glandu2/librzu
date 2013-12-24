#ifndef LOG_H
#define LOG_H

#include "Object.h"

struct TS_MESSAGE;

class RAPPELZLIB_EXTERN Log : public Object
{
public:
	enum Level {
		LL_Fatal,
		LL_Error,
		LL_Warning,
		LL_Info,
		LL_Debug,
		LL_Trace
	};

	Log();


	void packet(const TS_MESSAGE* message);
	void log(Level level, const char* message, ...);

	static Log* get();
};

#endif // LOG_H
