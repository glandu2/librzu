#ifndef THREADWORK_H
#define THREADWORK_H

#include "../Extern.h"
#include <vector>
#include "uv.h"

class RZU_EXTERN ThreadWorkBase
{
public:
	~ThreadWorkBase();

	int queue(uv_work_cb work_cb, uv_after_work_cb after_work_cb);
	int cancel();

protected:
	uv_work_t handle;
};

template<class CallbackClass>
class ThreadWork : public ThreadWorkBase
{
public:
	typedef void (CallbackClass::*ProcessCallback)();
	typedef void (CallbackClass::*DoneCallback)(int status);
public:
	ThreadWork() : inProgress(false) {
		handle.data = this;
	}
	~ThreadWork() {
		handle.data = nullptr;
	}

	int queue(CallbackClass* instance, ProcessCallback processCb, DoneCallback doneCb) {
		this->processCallback = processCb;
		this->doneCallback = doneCb;
		this->instance = instance;
		this->inProgress = true;
		return ThreadWorkBase::queue(&ThreadWork::onProcess, &ThreadWork::onDone);
	}

	bool isInProgress() { return inProgress; }

private:
	CallbackClass* instance;
	ProcessCallback processCallback;
	DoneCallback doneCallback;
	bool inProgress;

	static void onProcess(uv_work_t* handle) {
		if(handle->data) {
			ThreadWork *threadWork = static_cast<ThreadWork*>(handle->data);
			CallbackClass* instance = threadWork->instance;
			ProcessCallback callback = threadWork->processCallback;

			(instance->*callback)();
		}
	}

	static void onDone(uv_work_t* handle, int status) {
		if(handle->data) {
			ThreadWork *threadWork = static_cast<ThreadWork*>(handle->data);
			CallbackClass* instance = threadWork->instance;
			DoneCallback callback = threadWork->doneCallback;

			if(instance && callback)
				(instance->*callback)(status);
			threadWork->inProgress = false;
		}
	}
};

#endif // THREADWORK_H
