#include "Timer.h"
#include "EventLoop.h"

std::vector<uv_timer_t*> Timer::freeTimers;

Timer::Timer() {
	if(freeTimers.empty()) {
		uv_timer_t* newTimer = new uv_timer_t;
		uv_timer_init(EventLoop::getLoop(), newTimer);
		handle = newTimer;
	} else {
		handle = freeTimers.back();
		freeTimers.pop_back();
	}
}

Timer::~Timer() {
	stop();
	uv_close((uv_handle_t*)handle, &onClosedCallback);
}

int Timer::start(uv_timer_cb cb, uint64_t timeout, uint64_t repeat) {
	return uv_timer_start(handle, cb, timeout, repeat);
}

int Timer::stop() {
	return uv_timer_stop(handle);
}

int Timer::again() {
	return uv_timer_again(handle);
}

void Timer::setRepeat(uint64_t repeat) {
	uv_timer_set_repeat(handle, repeat);
}

uint64_t Timer::getRepeat() const {
	return uv_timer_get_repeat(handle);
}

void Timer::onClosedCallback(uv_handle_t *handle) {
	freeTimers.push_back((uv_timer_t*)handle);
}
