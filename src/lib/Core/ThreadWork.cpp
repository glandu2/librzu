#include "ThreadWork.h"
#include "EventLoop.h"

std::vector<uv_work_t*> ThreadWorkBase::freeHandles;

ThreadWorkBase::ThreadWorkBase() : handle(nullptr) {
}

ThreadWorkBase::~ThreadWorkBase() {
	if(handle) {
		cancel();
		handle->data = nullptr;
	}
}

int ThreadWorkBase::queue(uv_work_cb work_cb, uv_after_work_cb after_work_cb) {
	if(handle)
		return -EBUSY;

	if(freeHandles.empty()) {
		handle = new uv_work_t;
	} else {
		handle = freeHandles.back();
		freeHandles.pop_back();
	}
	handle->data = this;
	return uv_queue_work(EventLoop::getLoop(), handle, work_cb, after_work_cb);
}

int ThreadWorkBase::cancel() {
	return uv_cancel(reinterpret_cast<uv_req_t*>(handle));
}

void ThreadWorkBase::freeHandle()
{
	if(handle)
		freeHandles.push_back(handle);
	handle = nullptr;
}
