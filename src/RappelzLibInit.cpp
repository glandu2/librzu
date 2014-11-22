#include "RappelzLibInit.h"
#include "RappelzLibConfig.h"
#include "Log.h"
#include "CrashHandler.h"
#include <openssl/crypto.h>
#include "uv.h"

#ifdef __unix
#include <signal.h>
#endif

static void disableSigPipe() {
#ifdef __unix
	signal(SIGPIPE, SIG_IGN);
#endif
}


struct CRYPTO_dynlock_value {
	uv_mutex_t mutex;
};

static std::vector<uv_mutex_t> staticLocks;

static void OPENSSL_threadGetId(CRYPTO_THREADID* id) {
	CRYPTO_THREADID_set_numeric(id, uv_thread_self());
}

static void OPENSSL_staticLock(int mode, int type, const char *file, int line) {
	Log* logger = Log::get();

	if(mode & CRYPTO_LOCK) {
		if(logger)
			logger->log(Log::LL_Trace, "OPENSSL", 7, "Locking static lock %d\n", type);
		uv_mutex_lock(&staticLocks[type]);
	} else {
		if(logger)
			logger->log(Log::LL_Trace, "OPENSSL", 7, "Unlocking static lock %d\n", type);
		uv_mutex_unlock(&staticLocks[type]);
	}
}

struct CRYPTO_dynlock_value * OPENSSL_dyn_create_function(const char *file, int line) {
	struct CRYPTO_dynlock_value* lock = (struct CRYPTO_dynlock_value*) malloc(sizeof(struct CRYPTO_dynlock_value));
	uv_mutex_init(&lock->mutex);

	file = file;
	line = line;

	Log* logger = Log::get();
	if(logger)
		logger->log(Log::LL_Trace, "OPENSSL", 7, "Creating dynamic lock: %p\n", lock);

	return lock;
}

void OPENSSL_dyn_lock_function(int mode, struct CRYPTO_dynlock_value *l, const char *file, int line) {
	Log* logger = Log::get();

	file = file;
	line = line;

	if(mode & CRYPTO_LOCK) {
		if(logger)
			logger->log(Log::LL_Trace, "OPENSSL", 7, "Locking dynamic lock %p\n", l);
		uv_mutex_lock(&l->mutex);
	} else {
		if(logger)
			logger->log(Log::LL_Trace, "OPENSSL", 7, "Unlocking dynamic lock %p\n", l);
		uv_mutex_unlock(&l->mutex);
	}
}

void OPENSSL_dyn_destroy_function(struct CRYPTO_dynlock_value *l, const char *file, int line) {
	file = file;
	line = line;

	Log* logger = Log::get();
	if(logger)
		logger->log(Log::LL_Trace, "OPENSSL", 7, "Destroying dynamic lock: %p\n", l);

	free(l);
}

static void initOpenssl() {
	CRYPTO_THREADID_set_callback(&OPENSSL_threadGetId);

	staticLocks.resize(CRYPTO_num_locks());
	for(int i = 0; i < CRYPTO_num_locks(); i++) {
		uv_mutex_t* lock = &staticLocks[i];

		uv_mutex_init(lock);
	}
	CRYPTO_set_locking_callback(&OPENSSL_staticLock);

	CRYPTO_set_dynlock_create_callback(&OPENSSL_dyn_create_function);
	CRYPTO_set_dynlock_lock_callback(&OPENSSL_dyn_lock_function);
	CRYPTO_set_dynlock_destroy_callback(&OPENSSL_dyn_destroy_function);
}

bool RappelzLibInit() {
	disableSigPipe();
	CrashHandler::setProcessExceptionHandlers();
	CrashHandler::setThreadExceptionHandlers();

	initOpenssl();

	return true;
}
