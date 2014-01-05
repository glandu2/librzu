#ifndef LOG_H
#define LOG_H

#include <string>
#include "Object.h"
#include "uv.h"
#include "ConfigInfo.h"

struct TS_MESSAGE;

class RAPPELZLIB_EXTERN Log : public Object, public ICallbackGuard
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

	Log(cval<bool>& enabled, cval<std::string>& fileMaxLevel, cval<std::string>& consoleMaxLevel, cval<std::string>& dir, cval<std::string>& fileName);
	Log(cval<bool>& enabled, Level fileMaxLevel, Level consoleMaxLevel, cval<std::string>& dir, cval<std::string>& fileName);
	~Log();

	bool open();
	void close();

	void log(Level level, const char* objectName, size_t objectNameSize, const char* message, ...);
	void log(Level level, const char* objectName, size_t objectNameSize, const char* message, va_list args);

	static bool init();
	static Log* get() { return messageLogger; }
	static Log* getPacket() { return packetLogger; }

protected:
	static void updateEnabled(ICallbackGuard* instance, cval<bool>* level);
	static void updateFileLevel(ICallbackGuard* instance, cval<std::string>* level);
	static void updateConsoleLevel(ICallbackGuard* instance, cval<std::string>* level);
	static void updateFile(ICallbackGuard* instance, cval<std::string>* str);
	static void flushLogFile(uv_timer_t*timer, int status);

	void updateLevel(bool isConsole, const std::string& level);

private:
	static Log* messageLogger;
	static Log* packetLogger;

	Level fileMaxLevel;
	Level consoleMaxLevel;
	cval<std::string>& dir;
	cval<std::string>& fileName;
	std::string openedFile;
	void* file;
	uv_mutex_t lock;
	uv_timer_t flushTimer;
};

#endif // LOG_H
