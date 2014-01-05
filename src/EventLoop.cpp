#include "EventLoop.h"

uv_once_t EventLoop::tlsKeyInitOnce = UV_ONCE_INIT;
uv_key_t EventLoop::tlsKey;

void EventLoop::initKey() {
	uv_key_create(&tlsKey);
}

EventLoop::EventLoop() : loop(uv_loop_new())
{
	deleteObjectsHandle.data = this;
	uv_prepare_init(loop, &deleteObjectsHandle);
	uv_prepare_start(&deleteObjectsHandle, &deleteObjects);
}
EventLoop::~EventLoop() {
	if(uv_loop_alive(loop))
		warn("Loop still used but delete requested\n");
	uv_stop(loop);
	uv_loop_delete(loop);

	EventLoop* threadLoop = (EventLoop*) uv_key_get(&tlsKey);
	if(threadLoop == this)
		uv_key_set(&tlsKey, NULL);
}

EventLoop* EventLoop::getInstance() {
	EventLoop *threadLocalEventLoop;

	uv_once(&tlsKeyInitOnce, &initKey);

	threadLocalEventLoop = (EventLoop*) uv_key_get(&tlsKey);
	if(!threadLocalEventLoop) {
		threadLocalEventLoop = new EventLoop;
		uv_key_set(&tlsKey, threadLocalEventLoop);
	}

	return threadLocalEventLoop;
}

void EventLoop::deleteObjects(uv_prepare_t* handle, int status) {
	EventLoop* thisInstance = (EventLoop*)handle->data;

	std::list<Object*>::iterator it = thisInstance->objectsToDelete.begin();
	for(; it != thisInstance->objectsToDelete.end(); ) {
		delete *it;
		it = thisInstance->objectsToDelete.erase(it);
	}
}
