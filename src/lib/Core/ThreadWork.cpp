#include "ThreadWork.h"
#include "EventLoop.h"

ThreadWorkBase::~ThreadWorkBase() {
	cancel();
	handle.data = nullptr;
}

int ThreadWorkBase::queue(uv_work_cb work_cb, uv_after_work_cb after_work_cb) {
	return uv_queue_work(EventLoop::getLoop(), &handle, work_cb, after_work_cb);
}

int ThreadWorkBase::cancel() {
	return uv_cancel(reinterpret_cast<uv_req_t*>(&handle));
}
