#include "EventLoop.h"

uv_once_t EventLoop::tlsKeyInitOnce = UV_ONCE_INIT;
uv_key_t EventLoop::tlsKey;

void EventLoop::initKey() {
	uv_key_create(&tlsKey);
}

EventLoop::EventLoop() {
	uv_loop_init(&loop);
	deleteObjectsHandle.data = this;
	uv_prepare_init(&loop, &deleteObjectsHandle);
	uv_prepare_start(&deleteObjectsHandle, &staticDeleteObjects);
	uv_unref((uv_handle_t*) &deleteObjectsHandle);
}

EventLoop::~EventLoop() {
	if(uv_loop_alive(&loop))
		log(LL_Warning, "Loop still used but delete requested\n");
	uv_stop(&loop);
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

void EventLoop::deleteObjects() {
	std::list<Object*>::iterator it = objectsToDelete.begin();
	for(; it != objectsToDelete.end();) {
		delete *it;
		it = objectsToDelete.erase(it);
	}
}

void EventLoop::staticDeleteObjects(uv_prepare_t* handle) {
	static bool deletingObjects = false;
	EventLoop* thisInstance = (EventLoop*) handle->data;

	if(deletingObjects == false) {
		deletingObjects = true;
		thisInstance->deleteObjects();
		deletingObjects = false;
	}
}
