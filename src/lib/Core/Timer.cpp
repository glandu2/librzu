#include "Timer.h"
#include "EventLoop.h"

std::vector<uv_timer_t*> TimerBase::freeTimers;

TimerBase::TimerBase() {
	if(freeTimers.empty()) {
		handle = new uv_timer_t;
	} else {
		handle = freeTimers.back();
		freeTimers.pop_back();
	}
	uv_timer_init(EventLoop::getLoop(), handle);
}

TimerBase::~TimerBase() {
	stop();
	handle->data = nullptr;
	uv_close((uv_handle_t*)handle, &onClosedCallback);
}

int TimerBase::start(uv_timer_cb cb, uint64_t timeout, uint64_t repeat) {
	return uv_timer_start(handle, cb, timeout, repeat);
}

int TimerBase::stop() {
	return uv_timer_stop(handle);
}

int TimerBase::again() {
	return uv_timer_again(handle);
}

void TimerBase::setRepeat(uint64_t repeat) {
	uv_timer_set_repeat(handle, repeat);
}

uint64_t TimerBase::getRepeat() const {
	return uv_timer_get_repeat(handle);
}

void TimerBase::ref()
{
	uv_ref((uv_handle_t*)handle);
}

void TimerBase::unref()
{
	uv_unref((uv_handle_t*)handle);
}

void TimerBase::onClosedCallback(uv_handle_t *handle) {
	freeTimers.push_back((uv_timer_t*)handle);
}

void TimerStatic::setData(void *data) {
	handle->data = data;
}

void *TimerStatic::getData() const {
	return handle->data;
}
