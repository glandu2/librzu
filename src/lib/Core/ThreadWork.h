#ifndef THREADWORK_H
#define THREADWORK_H

#include "../Extern.h"
#include <vector>
#include "uv.h"

class RZU_EXTERN ThreadWorkBase
{
public:
	ThreadWorkBase();
	~ThreadWorkBase();

	int queue(uv_work_cb work_cb, uv_after_work_cb after_work_cb);
	int cancel();
	bool isInProgress() { return handle != nullptr; }

protected:
	void freeHandle();
private:
	uv_work_t* handle;

	static std::vector<uv_work_t*> freeHandles;
};

template<class CallbackClass>
class ThreadWork : public ThreadWorkBase
{
public:
	typedef void (CallbackClass::*ProcessCallback)();
	typedef void (CallbackClass::*DoneCallback)(int status);
public:
	int queue(CallbackClass* instance, ProcessCallback processCb, DoneCallback doneCb) {
		if(isInProgress)
			return -EBUSY;

		this->processCallback = processCb;
		this->doneCallback = doneCb;
		this->instance = instance;
		return ThreadWorkBase::queue(&ThreadWork::onProcess, &ThreadWork::onDone);
	}

private:
	CallbackClass* instance;
	ProcessCallback processCallback;
	DoneCallback doneCallback;

	static void onProcess(uv_work_t* handle) {
		if(handle && handle->data) {
			ThreadWork *threadWork = static_cast<ThreadWork*>(handle->data);
			CallbackClass* instance = threadWork->instance;
			ProcessCallback callback = threadWork->processCallback;

			(instance->*callback)();
		}
	}

	static void onDone(uv_work_t* handle, int status) {
		if(handle && handle->data) {
			ThreadWork *threadWork = static_cast<ThreadWork*>(handle->data);
			CallbackClass* instance = threadWork->instance;
			DoneCallback callback = threadWork->doneCallback;

			if(instance && callback)
				(instance->*callback)(status);
		}
		freeHandle();
	}
};

#endif // THREADWORK_H
