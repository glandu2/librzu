#ifndef LOG_H
#define LOG_H

#include "Object.h"
#include <string>

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

	Log(const bool& enabled, const Level& maxLevel, const std::string& file);


	void log(Level level, const char* message, ...);

	static Log* get();
	static Log* getPacket();

private:
	const bool& enabled;
	const Level& maxLevel;
	const std::string& filename;
};

#endif // LOG_H
