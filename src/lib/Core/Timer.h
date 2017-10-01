#ifndef TIMER_H
#define TIMER_H

#include "../Extern.h"
#include "uv.h"
#include <vector>

class RZU_EXTERN TimerBase {
public:
	TimerBase();
	~TimerBase();

	int start(uv_timer_cb cb, uint64_t timeout, uint64_t repeat);
	int stop();
	int again();
	void setRepeat(uint64_t repeat);
	uint64_t getRepeat() const;

	void ref();
	void unref();

protected:
	uv_timer_t* handle;

	static void onClosedCallback(uv_handle_t* handle);

	static std::vector<uv_timer_t*> freeTimers;
};

class RZU_EXTERN TimerStatic : public TimerBase {
public:
	void setData(void* data);
	void* getData() const;
};

template<class CallbackClass> class Timer : public TimerBase {
public:
	typedef void (CallbackClass::*TimeoutCallback)();

public:
	Timer() { handle->data = this; }
	~Timer() { handle->data = nullptr; }

	int start(CallbackClass* instance, TimeoutCallback cb, uint64_t timeout, uint64_t repeat) {
		this->callback = cb;
		this->instance = instance;
		return TimerBase::start(&Timer::onTimeout, timeout, repeat);
	}

private:
	CallbackClass* instance;
	TimeoutCallback callback;

	static void onTimeout(uv_timer_t* handle) {
		if(handle->data) {
			Timer* timer = (Timer*) handle->data;
			CallbackClass* instance = timer->instance;
			TimeoutCallback callback = timer->callback;

			(instance->*callback)();
		}
	}
};

#endif  // TIMER_H
