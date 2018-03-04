#include "EventLoop.h"

uv_once_t EventLoop::tlsKeyInitOnce = UV_ONCE_INIT;
uv_key_t EventLoop::tlsKey;

void EventLoop::initKey() {
	uv_key_create(&tlsKey);
}

EventLoop::EventLoop() : deletingObjects(false) {
	uv_loop_init(&loop);
	deleteObjectsHandle.data = this;
	uv_prepare_init(&loop, &deleteObjectsHandle);
	uv_prepare_start(&deleteObjectsHandle, &staticDeleteObjects);
	uv_unref((uv_handle_t*) &deleteObjectsHandle);
}

EventLoop::~EventLoop() {
	uv_ref((uv_handle_t*) &deleteObjectsHandle);
	uv_prepare_stop(&deleteObjectsHandle);
	uv_close((uv_handle_t*) &deleteObjectsHandle, nullptr);

	// Wait remaining handle to close
	uv_run(&loop, UV_RUN_DEFAULT);

	if(uv_loop_alive(&loop)) {
		log(LL_Warning, "Loop still used but delete requested, handles:\n");
		uv_walk(&loop,
		        [](uv_handle_t* handle, void* arg) {
			        EventLoop* self = (EventLoop*) arg;
			        const char* type;
			        switch(handle->type) {
#define X(uc, lc) \
	case UV_##uc: \
		type = #lc; \
		break;
				        UV_HANDLE_TYPE_MAP(X)
#undef X
				        default:
					        type = "<unknown>";
			        }

			        self->log(LL_Warning, "  [%08X] %s %p\n", handle->flags, type, handle);
		        },
		        this);
	}
	uv_loop_close(&loop);

	EventLoop* threadLoop = (EventLoop*) uv_key_get(&tlsKey);
	if(threadLoop == this)
		uv_key_set(&tlsKey, NULL);

	deleteObjects();
}

EventLoop* EventLoop::getInstance() {
	EventLoop* threadLocalEventLoop;

	uv_once(&tlsKeyInitOnce, &initKey);

	threadLocalEventLoop = (EventLoop*) uv_key_get(&tlsKey);
	if(!threadLocalEventLoop) {
		threadLocalEventLoop = new EventLoop;
		uv_key_set(&tlsKey, threadLocalEventLoop);
	}

	return threadLocalEventLoop;
}

void EventLoop::deinit() {
	EventLoop* threadLocalEventLoop;

	uv_once(&tlsKeyInitOnce, &initKey);

	threadLocalEventLoop = (EventLoop*) uv_key_get(&tlsKey);
	if(threadLocalEventLoop) {
		delete threadLocalEventLoop;
	}
}

void EventLoop::deleteObjects() {
	if(deletingObjects == false) {
		deletingObjects = true;

		std::list<Object*>::iterator it = objectsToDelete.begin();
		for(; it != objectsToDelete.end();) {
			delete *it;
			it = objectsToDelete.erase(it);
		}

		deletingObjects = false;
	}
}

void EventLoop::staticDeleteObjects(uv_prepare_t* handle) {
	EventLoop* thisInstance = (EventLoop*) handle->data;

	thisInstance->deleteObjects();
	uv_unref((uv_handle_t*) handle);
}
