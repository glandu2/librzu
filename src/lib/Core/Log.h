#ifndef LOG_H
#define LOG_H

#include "IListener.h"
#include "Object.h"
#include "uv.h"
#include <string>
#include <vector>

template<typename T> class cval;

class RZU_EXTERN Log : public Object, public IListener {
	DECLARE_CLASS(Log)
public:
	Log(cval<bool>& enabled,
	    cval<std::string>& fileMaxLevel,
	    cval<std::string>& consoleMaxLevel,
	    cval<std::string>& dir,
	    cval<std::string>& fileName,
	    cval<int>& maxQueueSize);
	Log(cval<bool>& enabled,
	    Level fileMaxLevel,
	    Level consoleMaxLevel,
	    cval<std::string>& dir,
	    cval<std::string>& fileName,
	    cval<int>& maxQueueSize);

	~Log();

	void startWriter();
	void stopWriter(bool waitThread = true);

	uv_thread_t getLogWriterThread() { return logWritterThreadId; }

	void log(Level level, Object* object, const char* message, ...) PRINTFCHECK(4, 5);
	void logv(Level level, Object* object, const char* message, va_list args);
	void log(Level level, const char* objectName, size_t objectNameSize, const char* message, ...) PRINTFCHECK(5, 6);
	void logv(Level level, const char* objectName, size_t objectNameSize, const char* message, va_list args);

	static Log* get() { return defaultLogger; }
	static Log* setDefaultLogger(Log* newLogger) {
		Log* old = defaultLogger;
		defaultLogger = newLogger;
		return old;
	}
	static Log* getDefaultPacketLogger() { return defaultPacketLogger; }
	static Log* setDefaultPacketLogger(Log* newLogger) {
		Log* old = defaultPacketLogger;
		defaultPacketLogger = newLogger;
		return old;
	}

	Level getMaxLevel() { return fileMaxLevel > consoleMaxLevel ? fileMaxLevel : consoleMaxLevel; }

	bool wouldLog(Level level) { return (level <= fileMaxLevel || level <= consoleMaxLevel) && stop == false; }

	size_t getQueueUsage();

protected:
	static void updateEnabled(IListener* instance, cval<bool>* level);
	static void updateFileLevel(IListener* instance, cval<std::string>* level);
	static void updateConsoleLevel(IListener* instance, cval<std::string>* level);
	static void updateFile(IListener* instance, cval<std::string>* str);

	static void logWritterThreadStatic(void* arg);
	void logWritterThread();

	void updateLevel(bool isConsole, const std::string& level);

private:
	struct Message {
		time_t time;
		std::string objectName;
		std::string message;
		Level level;
		bool writeToConsole;
		bool writeToFile;
	};

	void construct(cval<bool>& enabled, cval<std::string>& dir, cval<std::string>& fileName);

	static Log* defaultLogger;
	static Log* defaultPacketLogger;

	uv_thread_t logWritterThreadId;
#ifdef _WIN32
	unsigned long logWritterThreadNativeId;
#endif
	bool logWritterThreadStarted;

	uv_mutex_t messageListMutex;
	uv_cond_t messageListCond;
	volatile bool stop;
	std::vector<Message*> messageQueue;

	Level fileMaxLevel;
	Level consoleMaxLevel;
	cval<std::string>& dir;
	cval<std::string>& fileName;
	cval<int>& maxQueueSize;
	bool updateFileRequested;
	bool messageQueueFull;
	size_t maxQueueSizeReached;
};

#endif  // LOG_H
