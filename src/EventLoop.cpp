#include "EventLoop.h"

EventLoop::EventLoop() : loop(uv_loop_new())
{
	deleteObjectsHandle.data = this;
	uv_prepare_init(loop, &deleteObjectsHandle);
	uv_prepare_start(&deleteObjectsHandle, &deleteObjects);
}

EventLoop* EventLoop::getInstance() {
	static EventLoop globalEventLoop;

	return &globalEventLoop;
}

void EventLoop::deleteObjects(uv_prepare_t* handle, int status) {
	EventLoop* thisInstance = (EventLoop*)handle->data;

	std::list<Object*>::iterator it = thisInstance->objectsToDelete.begin();
	for(; it != thisInstance->objectsToDelete.end(); ) {
		delete *it;
		it = thisInstance->objectsToDelete.erase(it);
	}
}
