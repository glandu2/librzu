#include "LibRzuInit.h"
#include "CrashHandler.h"
#include <openssl/crypto.h>
#include <openssl/err.h>
#include "uv.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

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

static void OPENSSL_staticLock(int mode, int type, const char *file, int line) {
	if(mode & CRYPTO_LOCK) {
		uv_mutex_lock(&staticLocks[type]);
	} else {
		uv_mutex_unlock(&staticLocks[type]);
	}
}

struct CRYPTO_dynlock_value * OPENSSL_dyn_create_function(const char *, int) {
	struct CRYPTO_dynlock_value* lock = new struct CRYPTO_dynlock_value;
	uv_mutex_init(&lock->mutex);

	return lock;
}

void OPENSSL_dyn_lock_function(int mode, struct CRYPTO_dynlock_value *l, const char*, int) {
	if(mode & CRYPTO_LOCK) {
		uv_mutex_lock(&l->mutex);
	} else {
		uv_mutex_unlock(&l->mutex);
	}
}

void OPENSSL_dyn_destroy_function(struct CRYPTO_dynlock_value *l, const char *, int) {
	uv_mutex_destroy(&l->mutex);
	delete l;
}

static void initOpenssl() {
	CRYPTO_set_id_callback(&uv_thread_self);

	staticLocks.resize(CRYPTO_num_locks());
	for(int i = 0; i < CRYPTO_num_locks(); i++) {
		uv_mutex_t* lock = &staticLocks[i];

		uv_mutex_init(lock);
	}
	CRYPTO_set_locking_callback(&OPENSSL_staticLock);

	CRYPTO_set_dynlock_create_callback(&OPENSSL_dyn_create_function);
	CRYPTO_set_dynlock_lock_callback(&OPENSSL_dyn_lock_function);
	CRYPTO_set_dynlock_destroy_callback(&OPENSSL_dyn_destroy_function);

	ERR_load_crypto_strings();
}

bool LibRzuInit() {
	srand((unsigned int)time(NULL));

	disableSigPipe();
	CrashHandler::setProcessExceptionHandlers();
	CrashHandler::setThreadExceptionHandlers();

	initOpenssl();

	return true;
}
