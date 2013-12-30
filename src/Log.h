#ifndef LOG_H
#define LOG_H

#include <string>
#include "Object.h"
#include "uv.h"
#include "ConfigInfo.h"

struct TS_MESSAGE;

class RAPPELZLIB_EXTERN Log : public Object
{
	DECLARE_CLASS(Log)
public:
	enum Level {
		LL_Fatal,
		LL_Error,
		LL_Warning,
		LL_Info,
		LL_Debug,
		LL_Trace
	};

	Log(cval<bool>& enabled, cval<std::string>& maxLevel, cval<std::string>& dir, cval<std::string>& fileName);
	Log(cval<bool>& enabled, Level maxLevel, cval<std::string>& dir, cval<std::string>& fileName);
	~Log();


	void log(Level level, const char* objectName, const char* message, ...);
	void log(Level level, const char* objectName, const char* message, va_list args);

	static bool init();
	static Log* get() { return messageLogger; }
	static Log* getPacket() { return packetLogger; }

protected:
	static void updateLevel(void* instance, cval<std::string>* level);
	static void updateFile(void* instance, cval<std::string>* str);

private:
	static Log* messageLogger;
	static Log* packetLogger;

	cval<bool>& enabled;
	Level maxLevel;
	cval<std::string>& dir;
	cval<std::string>& fileName;
	void* file;
	uv_mutex_t lock;
};

#endif // LOG_H
