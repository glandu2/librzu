#include "Log.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include "Utils.h"
#include "Config/ConfigParamVal.h"

static const char * const  LEVELSTRINGS[] = { "FATAL", "ERROR", "Warn", "Info", "Debug", "Trace" };

Log* Log::defaultLogger = nullptr;
Log* Log::defaultPacketLogger = nullptr;

Log::Log(cval<bool>& enabled, cval<std::string>& fileMaxLevel, cval<std::string>& consoleMaxLevel, cval<std::string>& dir, cval<std::string>& fileName, cval<int>& maxQueueSize) :
	fileMaxLevel(LL_Info),
	consoleMaxLevel(LL_Info),
	dir(dir),
	fileName(fileName),
	maxQueueSize(maxQueueSize),
	maxQueueSizeReached(0)
{
	updateFileLevel(this, &fileMaxLevel);
	updateConsoleLevel(this, &consoleMaxLevel);

	construct(enabled, dir, fileName);

	fileMaxLevel.addListener(this, &updateFileLevel);
	consoleMaxLevel.addListener(this, &updateConsoleLevel);
}

Log::Log(cval<bool>& enabled, Level fileMaxLevel, Level consoleMaxLevel, cval<std::string>& dir, cval<std::string>& fileName, cval<int>& maxQueueSize) :
	fileMaxLevel(LL_Info),
	consoleMaxLevel(LL_Info),
	dir(dir),
	fileName(fileName),
	maxQueueSize(maxQueueSize),
	maxQueueSizeReached(0)
{
	this->fileMaxLevel = fileMaxLevel;
	this->consoleMaxLevel = consoleMaxLevel;

	construct(enabled, dir, fileName);
}

void Log::construct(cval<bool>& enabled, cval<std::string>& dir, cval<std::string>& fileName) {
	this->stop = true;
	this->logWritterThreadStarted = false;
	uv_mutex_init(&this->messageListMutex);
	uv_cond_init(&this->messageListCond);

	enabled.addListener(this, &updateEnabled);
	dir.addListener(this, &updateFile);
	fileName.addListener(this, &updateFile);

	updateEnabled(this, &enabled);
}

Log::~Log() {
	stopWriter();
	if(defaultLogger == this)
		defaultLogger = nullptr;
	uv_mutex_destroy(&messageListMutex);
}

void Log::updateEnabled(IListener* instance, cval<bool>* enable) {
	Log* thisInstance = (Log*) instance;

	if(enable->get()) {
		thisInstance->startWriter();
	} else {
		thisInstance->stopWriter(false);
	}
}

void Log::updateFileLevel(IListener* instance, cval<std::string>* level) {
	Log* thisInstance = (Log*) instance;

	thisInstance->updateLevel(false, level->get());
}

void Log::updateConsoleLevel(IListener* instance, cval<std::string>* level) {
	Log* thisInstance = (Log*) instance;

	thisInstance->updateLevel(true, level->get());
}

void Log::updateLevel(bool isConsole, const std::string& level) {
	Level *levelToChange;

	if(isConsole)
		levelToChange = &consoleMaxLevel;
	else
		levelToChange = &fileMaxLevel;

	if(level == "fatal" || level == "never")
		*levelToChange = LL_Fatal;
	else if(level == "error")
		*levelToChange = LL_Error;
	else if(level == "warning" || level == "warn")
		*levelToChange = LL_Warning;
	else if(level == "info")
		*levelToChange = LL_Info;
	else if(level == "debug")
		*levelToChange = LL_Debug;
	else if(level == "trace")
		*levelToChange = LL_Trace;
	else {
		const char* target;
		if(!isConsole)
			target = "file";
		else
			target = "console";

		Object::log(LL_Error, "Invalid %s level value: %s. Using warning. (valid ones are: fatal, never (alias for fatal), error, warning, info, debug and trace)\n", target, level.c_str());
		*levelToChange = LL_Warning;
	}

	if(isConsole)
		Object::log(LL_Debug, "Using console log level %s\n", LEVELSTRINGS[*levelToChange]);
	else
		Object::log(LL_Debug, "Using file log level %s\n", LEVELSTRINGS[*levelToChange]);
}

void Log::updateFile(IListener* instance, cval<std::string>* str) {
	Log* thisInstance = (Log*) instance;

	thisInstance->updateFileRequested = true;
}

void Log::startWriter() {
	if(this->stop == false)
		return;

	this->stop = false;
	this->updateFileRequested = true;
	this->messageQueueFull = false;
	int result = uv_thread_create(&this->logWritterThreadId, &logWritterThreadStatic, this);
	if(result == 0) {
		this->logWritterThreadStarted = true;
		log(LL_Info, this, "Log thread started using filename %s\n", fileName.get().c_str());
	} else {
		log(LL_Info, this, "Failed to start log thread: %d\n", result);
	}
}

void Log::stopWriter(bool waitThread) {
	if(this->stop == true) {
		return;
	}

#ifdef _WIN32
	if(!this->logWritterThreadStarted || this->logWritterThreadNativeId == GetCurrentThreadId())
#else /* unix */
	if(!this->logWritterThreadStarted || pthread_equal(pthread_self(), this->logWritterThreadId))
#endif
	{
		this->stop = true;
		return;
	}

	Object::log(LL_Debug, "Stopping log thread\n");
	log(LL_Info, this, "Log thread stopped\n");

	uv_mutex_lock(&this->messageListMutex);
	this->stop = true;
	uv_cond_signal(&this->messageListCond);
	uv_mutex_unlock(&this->messageListMutex);

	if(waitThread)
		uv_thread_join(&this->logWritterThreadId);
}


void Log::log(Level level, Object* object, const char* message, ...) {
	size_t nameSize;
	const char* name;
	va_list args;

	if(!wouldLog(level))
		return;

	name = object->getObjectName(&nameSize);

	va_start(args, message);
	logv(level, name, nameSize, message, args);
	va_end(args);

}

void Log::logv(Level level, Object* object, const char* message, va_list args) {
	size_t nameSize;
	const char* name;

	if(!wouldLog(level))
		return;

	name = object->getObjectName(&nameSize);
	logv(level, name, nameSize, message, args);

}

void Log::log(Level level, const char *objectName, size_t objectNameSize, const char* message, ...) {
	va_list args;

	if(!wouldLog(level))
		return;

	va_start(args, message);
	logv(level, objectName, objectNameSize, message, args);
	va_end(args);
}

void Log::logv(Level level, const char *objectName, size_t objectNameSize, const char* message, va_list args) {
	if(!wouldLog(level) || this->messageQueueFull)
		return;

	Message* msg = new Message;
	msg->time = time(NULL);
	msg->level = level;
	msg->writeToConsole = level <= consoleMaxLevel;
	msg->writeToFile = level <= fileMaxLevel;
	msg->objectName = std::string(objectName, objectName + objectNameSize);
	Utils::stringFormatv(msg->message, message, args);

	uv_mutex_lock(&this->messageListMutex);
	if((int)this->messageQueue.size() < maxQueueSize.get())
		this->messageQueue.push_back(msg);
	else
		this->messageQueueFull = true;
	if(this->messageQueue.size() > maxQueueSizeReached)
		maxQueueSizeReached = this->messageQueue.size();
	uv_cond_signal(&this->messageListCond);
	uv_mutex_unlock(&this->messageListMutex);
}

size_t Log::getQueueUsage() {
	return maxQueueSizeReached;
}

/*************************************/
/* In a thread                       */
/*************************************/

static FILE* openLogFile(FILE* currentFile, const std::string& dir, const std::string& filename, int year, int month, int day) {
	std::string absoluteDir = dir + '/';
	std::string newFileName = filename;
	char datesuffix[16];
	FILE* newfile;

	Utils::mkdir(absoluteDir.c_str());

	sprintf(datesuffix, "_%04d-%02d-%02d", year, month, day);
	newFileName.insert(newFileName.find_last_of('.'), datesuffix);
	newFileName = absoluteDir + newFileName;

	newfile = fopen(newFileName.c_str(), "at");
	if(!newfile) {
		return currentFile;
	} else {
		if(currentFile)
			fclose(currentFile);

		currentFile = newfile;
	}

	return currentFile;
}


static uv_mutex_t consoleMutex;
static uv_once_t initMutexOnce = UV_ONCE_INIT;
static void initMutex() {
	uv_mutex_init(&consoleMutex);
}

void Log::logWritterThreadStatic(void* arg) { reinterpret_cast<Log*>(arg)->logWritterThread(); }
void Log::logWritterThread() {
	std::vector<Message*>* messagesToWrite = new std::vector<Message*>;
	size_t i, size;
	struct tm localtm;
	std::vector<char> logHeader;

	bool endLoop = false;
	bool willUpdateFile;

	FILE* logFile = nullptr;
	int lastYear = -1;
	int lastMonth = -1;
	int lastDay = -1;

#ifdef _WIN32
	this->logWritterThreadNativeId = GetCurrentThreadId();
#endif
	this->logWritterThreadStarted = true;

	uv_once(&initMutexOnce, &initMutex);

	while(endLoop == false) {
		size_t pendingMessages;

		uv_mutex_lock(&this->messageListMutex);
		pendingMessages = this->messageQueue.size();
		uv_mutex_unlock(&this->messageListMutex);

		// Flush only if we will wait
		if(pendingMessages == 0 && logFile)
			fflush(logFile);

		uv_mutex_lock(&this->messageListMutex);
		while(this->messageQueue.size() == 0 && this->stop == false) {
			uv_cond_wait(&this->messageListCond, &this->messageListMutex);
		}

		endLoop = this->stop;
		willUpdateFile = this->updateFileRequested;
		messagesToWrite->swap(this->messageQueue);
		this->messageQueueFull = false;

		uv_mutex_unlock(&this->messageListMutex);


		size = messagesToWrite->size();

		bool messageUseFile = false;
		for(i = 0; i < size; i++) {
			if((*messagesToWrite)[i]->writeToFile) {
				messageUseFile = true;
				break;
			}
		}

		//Check if the date changed, if so, update the log file to us a new one (filename has timestamp)
		if(size > 0 && messageUseFile) {
			time_t firstMsgTime = messagesToWrite->at(0)->time;

			Utils::getGmTime(firstMsgTime, &localtm);

			if(localtm.tm_year != lastYear) {
				willUpdateFile = true;
				lastYear = localtm.tm_year;
			}
			if(localtm.tm_mon != lastMonth) {
				willUpdateFile = true;
				lastMonth = localtm.tm_mon;
			}
			if(localtm.tm_mday != lastDay) {
				willUpdateFile = true;
				lastDay = localtm.tm_mday;
			}

			if(willUpdateFile || logFile == nullptr) {
				this->updateFileRequested = false;

				FILE* newfile = openLogFile(logFile, this->dir.get(), this->fileName.get(), lastYear, lastMonth, lastDay);
				if(newfile == logFile) {
					if(logFile)
						fprintf(logFile, "Failed to change log file to %s\n", this->fileName.get().c_str());
					fprintf(stdout, "Failed to change log file to %s\n", this->fileName.get().c_str());
				}
				logFile = newfile;
				if(logFile)
					setvbuf(logFile, nullptr, _IOFBF, 64*1024);
			}
		}

		for(i = 0; i < size; i++) {
			const Message* const msg = messagesToWrite->at(i);

			Utils::getGmTime(msg->time, &localtm);

			//26 char to %-5s included
			logHeader.resize(27 + msg->objectName.size() + 3);
			size_t strLen = snprintf(&logHeader[0], logHeader.size(), "%4d-%02d-%02d %02d:%02d:%02d %-5s %s: ", localtm.tm_year, localtm.tm_mon, localtm.tm_mday, localtm.tm_hour, localtm.tm_min, localtm.tm_sec, LEVELSTRINGS[msg->level], msg->objectName.c_str());
			if(strLen >= logHeader.size()) {
				uv_mutex_lock(&consoleMutex);
					fprintf(stdout, "------------------- ERROR Log::logWritterThread: Log buffer was too small, next log message might be truncated\n");
				uv_mutex_unlock(&consoleMutex);
				strLen = logHeader.size()-1; //do not write the \0
			}

			if(msg->writeToConsole) {
				uv_mutex_lock(&consoleMutex);
					fwrite(&logHeader[0], 1, strLen, stdout);
					fwrite(msg->message.c_str(), 1, msg->message.size(), stdout);
				uv_mutex_unlock(&consoleMutex);
			}

			if(logFile && msg->writeToFile) {
				fwrite(&logHeader[0], 1, strLen, logFile);
				fwrite(msg->message.c_str(), 1, msg->message.size(), logFile);
			}

			delete msg;
		}

		messagesToWrite->clear();
	}

	delete messagesToWrite;
	if(logFile)
		fclose(logFile);
}
