#include "Log.h"
#include "RappelzLibConfig.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include "Utils.h"
#include "EventLoop.h"

static const char * const  LEVELSTRINGS[] = { "FATAL", "ERROR", "Warn", "Info", "Debug", "Trace" };

Log* Log::defaultLogger = nullptr;

Log::Log(cval<bool>& enabled, cval<std::string>& fileMaxLevel, cval<std::string>& consoleMaxLevel, cval<std::string>& dir, cval<std::string>& fileName) :
	fileMaxLevel(LL_Info),
	consoleMaxLevel(LL_Info),
	dir(dir),
	fileName(fileName),
	file(nullptr)
{
	construct(enabled, dir, fileName);

	updateFileLevel(this, &fileMaxLevel);
	updateConsoleLevel(this, &consoleMaxLevel);

	fileMaxLevel.addListener(this, &updateFileLevel);
	consoleMaxLevel.addListener(this, &updateConsoleLevel);
}

Log::Log(cval<bool>& enabled, Level fileMaxLevel, Level consoleMaxLevel, cval<std::string>& dir, cval<std::string>& fileName) :
	fileMaxLevel(LL_Info),
	consoleMaxLevel(LL_Info),
	dir(dir),
	fileName(fileName),
	file(nullptr)
{
	construct(enabled, dir, fileName);

	this->fileMaxLevel = fileMaxLevel;
	this->consoleMaxLevel = consoleMaxLevel;
}

void Log::construct(cval<bool>& enabled, cval<std::string>& dir, cval<std::string>& fileName) {
	this->stop = true;
	uv_mutex_init(&this->fileMutex);
	uv_mutex_init(&this->messageListMutex);
	uv_cond_init(&this->messageListCond);
	startWriter();

	updateEnabled(this, &enabled);

	enabled.addListener(this, &updateEnabled);
	dir.addListener(this, &updateFile);
	fileName.addListener(this, &updateFile);
}

Log::~Log() {
	close();
	stopWriter();
}

void Log::updateEnabled(IListener* instance, cval<bool>* enable) {
	Log* thisInstance = (Log*) instance;

	if(enable->get()) {
		thisInstance->open();
	} else {
		thisInstance->close();
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

	if(level == "fatal")
		*levelToChange = LL_Fatal;
	else if(level == "error")
		*levelToChange = LL_Error;
	else if(level == "warning")
		*levelToChange = LL_Warning;
	else if(level == "info")
		*levelToChange = LL_Info;
	else if(level == "debug")
		*levelToChange = LL_Debug;
	else if(level == "trace")
		*levelToChange = LL_Trace;
	else {
		error("Invalid level value: %s. Using warning.\n", level.c_str());
		*levelToChange = LL_Warning;
	}

	if(isConsole)
		debug("Using console log level %s\n", LEVELSTRINGS[*levelToChange]);
	else
		debug("Using file log level %s\n", LEVELSTRINGS[*levelToChange]);
}

void Log::updateFile(IListener* instance, cval<std::string>* str) {
	Log* thisInstance = (Log*) instance;

	thisInstance->open();
}

void Log::startWriter() {
	if(this->stop == false)
		return;

	this->stop = false;
	uv_thread_create(&this->logWritterThreadId, &logWritterThreadStatic, this);
}

void Log::stopWriter() {
	if(this->stop == true)
		return;

#ifdef _WIN32
	if(GetProcessId((HANDLE)this->logWritterThreadId) == GetCurrentProcessId())
#else /* unix */
	if(pthread_equal(pthread_self(), this->logWritterThreadId))
#endif
	{
		this->stop = true;
		return;
	}

	uv_mutex_lock(&this->messageListMutex);
	this->stop = true;
	uv_cond_signal(&this->messageListCond);
	uv_mutex_unlock(&this->messageListMutex);

	printf("Wait log thread\n");
	uv_thread_join(&this->logWritterThreadId);
}

bool Log::open() {
	std::string absoluteDir = dir.get();
	std::string newFileName = absoluteDir + "/" + fileName.get();
	FILE* newfile;

	Utils::mkdir(absoluteDir.c_str());

	newfile = fopen(newFileName.c_str(), "ab");
	if(!newfile) {
		if(this->file)
			error("Couldnt open new log file %s, keeping old one\n", newFileName.c_str());
		else
			error("Couldnt open log file %s for writting !\n", newFileName.c_str());
		return false;
	} else {
		uv_mutex_lock(&this->fileMutex);

		if(this->file) {
			info("Switching log file to %s\n", newFileName.c_str());


			fclose((FILE*)this->file);
			this->file = newfile;

		} else {
			this->file = newfile;
		}

		uv_mutex_unlock(&this->fileMutex);

		log(LL_Info, getObjectName(), getObjectNameSize(), "Log file \"%s\" opened\n", newFileName.c_str());
		return true;
	}
}

void Log::close() {
	info("Closing log file\n");
	if(defaultLogger == this)
		defaultLogger = nullptr;
	uv_mutex_lock(&this->fileMutex);
	if(this->file)
		fclose((FILE*)this->file);
	this->file = nullptr;
	uv_mutex_unlock(&this->fileMutex);
}

void Log::log(Level level, const char *objectName, size_t objectNameSize, const char* message, ...) {
	va_list args;

	if(level > fileMaxLevel && level > consoleMaxLevel)
		return;

	va_start(args, message);
	log(level, objectName, objectNameSize, message, args);
	va_end(args);
}

int c99vsnprintf(char* dest, int size, const char* format, va_list args) {
	va_list argsForCount;
	va_copy(argsForCount, args);

	int result = vsnprintf(dest, size, format, args);

#ifdef _WIN32
	if(result == -1)
		result = _vscprintf(format, argsForCount);
#endif

	return result;
}

void stringformat(std::string& dest, const char* message, va_list args) {
	va_list argsFor2ndPass;
	va_copy(argsFor2ndPass, args);

	dest.resize(128);
	int result = c99vsnprintf(&dest[0], dest.size(), message, args);

	if(result < 0) {
		dest = message;
		return;
	}

	if(result < (int)dest.size()) {
		dest.resize(result);
	} else if(result >= (int)dest.size()) {
		dest.resize(result+1);

		vsnprintf(&dest[0], dest.size(), message, argsFor2ndPass);
		dest.resize(result);
	}
}

void Log::log(Level level, const char *objectName, size_t objectNameSize, const char* message, va_list args) {
	if(level > fileMaxLevel && level > consoleMaxLevel)
		return;

	Message* msg = new Message;
	msg->time = time(NULL);
	msg->level = level;
	msg->writeToConsole = level <= consoleMaxLevel;
	msg->writeToFile = level <= fileMaxLevel;
	msg->objectName = std::string(objectName, objectName + objectNameSize);
	stringformat(msg->message, message, args);

	uv_mutex_lock(&this->messageListMutex);
	this->messageQueue.push_back(msg);
	uv_cond_signal(&this->messageListCond);
	uv_mutex_unlock(&this->messageListMutex);
}

void Log::logWritterThreadStatic(void* arg) { reinterpret_cast<Log*>(arg)->logWritterThread(); }
void Log::logWritterThread() {
	std::vector<Message*>* messagesToWrite = new std::vector<Message*>;
	size_t i, size;
	struct tm localtm;
	std::vector<char> logHeader;
	bool endLoop = false;

	while(endLoop == false) {
		uv_mutex_lock(&this->messageListMutex);

		while(this->messageQueue.size() == 0 && this->stop == false) {
			uv_cond_wait(&this->messageListCond, &this->messageListMutex);
		}

		endLoop = this->stop;
		messagesToWrite->swap(this->messageQueue);

		uv_mutex_unlock(&this->messageListMutex);


		uv_mutex_lock(&this->fileMutex);

		size = messagesToWrite->size();
		for(i = 0; i < size; i++) {
			const Message* const msg = messagesToWrite->at(i);

			Utils::getGmTime(msg->time, &localtm);

			//26 char to %-5s included
			logHeader.resize(27 + msg->objectName.size() + 3);
			size_t strLen = snprintf(&logHeader[0], logHeader.size(), "%4d-%02d-%02d %02d:%02d:%02d %-5s %s: ", localtm.tm_year, localtm.tm_mon, localtm.tm_mday, localtm.tm_hour, localtm.tm_min, localtm.tm_sec, LEVELSTRINGS[msg->level], msg->objectName.c_str());
			if(strLen >= logHeader.size()) {
				fprintf(stderr, "------------------- ERROR Log::logWritterThread: Log buffer was too small, next log message might be truncated\n");
				strLen = logHeader.size()-1; //do not write the \0
			}

			if(msg->writeToConsole) {
				fwrite(&logHeader[0], 1, strLen, stderr);
				fwrite(msg->message.c_str(), 1, msg->message.size(), stderr);
			}

			if(this->file && msg->writeToFile) {
				fwrite(&logHeader[0], 1, strLen, (FILE*)this->file);
				fwrite(msg->message.c_str(), 1, msg->message.size(), (FILE*)this->file);
			}

			delete msg;
		}

		fflush((FILE*)this->file);

		uv_mutex_unlock(&this->fileMutex);

		messagesToWrite->clear();
	}

	delete messagesToWrite;
}

bool Log::isAllMessageWritten() {
	return true;
}
