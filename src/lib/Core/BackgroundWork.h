#pragma once

#include "../Extern.h"
#include "Core/EventLoop.h"
#include "Core/Utils.h"
#include "uv.h"
#include <array>
#include <tuple>

template<class CallbackClass, class Argument> class BackgroundWork {
public:
	typedef int (CallbackClass::*WorkCallback)(Argument arg);
	typedef void (CallbackClass::*AfterWorkCallback)(Argument arg, int status);

public:
	BackgroundWork(CallbackClass* targetInstance, WorkCallback wcb, AfterWorkCallback awcb) {
		this->instance = targetInstance;
		this->workCallback = wcb;
		this->afterWorkCallback = awcb;

		work.data = this;
		uv_mutex_init(&stopMutex);
	}
	~BackgroundWork() {
		uv_cancel((uv_req_t*) &work);
		uv_mutex_lock(&stopMutex);
		work.data = nullptr;
		uv_mutex_unlock(&stopMutex);
		uv_mutex_destroy(&stopMutex);
	}

	int run(Argument arg) {
		if(uv_mutex_trylock(&stopMutex) < 0)
			return UV_EBUSY;

		this->savedArg = arg;
		this->result = 0;

		uv_queue_work(EventLoop::getLoop(), &work, &onWork, &onAfterWork);

		return 0;
	}

private:
	static void onWork(uv_work_t* req) {
		BackgroundWork* thisInstance = static_cast<BackgroundWork*>(req->data);
		// uint64_t startTime = Utils::getTimeInMsec();
		thisInstance->result = (thisInstance->instance->*thisInstance->workCallback)(thisInstance->savedArg);
		// thisInstance->instance->log(Object::LL_Info, "Work done in %d\n", Utils::getTimeInMsec() - startTime);
	}

	static void onAfterWork(uv_work_t* req, int status) {
		BackgroundWork* thisInstance = static_cast<BackgroundWork*>(req->data);
		(thisInstance->instance->*thisInstance->afterWorkCallback)(
		    std::move(thisInstance->savedArg), thisInstance->result ? thisInstance->result : status);
		uv_mutex_unlock(&thisInstance->stopMutex);
	}

private:
	CallbackClass* instance;
	WorkCallback workCallback;
	AfterWorkCallback afterWorkCallback;

	Argument savedArg;
	int result;

	uv_mutex_t stopMutex;
	uv_work_t work;
};

