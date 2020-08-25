#pragma once

#include "../Extern.h"
#include "Core/EventLoop.h"
#include "Core/Utils.h"
#include "uv.h"
#include <array>
#include <list>
#include <memory>
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
	}
	~BackgroundWork() { cancel(); }

	void cancel() {
		for(auto& job : ongoingJobs) {
			uv_cancel((uv_req_t*) &job->work);
			job->instance = nullptr;
		}
		ongoingJobs.clear();
	}

	int run(Argument arg) {
		std::unique_ptr<JobInstance> job;

		if(freeJobs.empty()) {
			job.reset(new JobInstance);
		} else {
			job = std::move(freeJobs.back());
			freeJobs.pop_back();
		}

		job->instance = this;
		job->savedArg = arg;
		job->result = EINTR;
		job->work.data = job.get();
		auto* work = &job->work;

		ongoingJobs.push_back(job.release());

		uv_queue_work(EventLoop::getLoop(), work, &onWork, &onAfterWork);

		return 0;
	}

private:
	static void onWork(uv_work_t* req) {
		JobInstance* jobInstance = static_cast<JobInstance*>(req->data);
		BackgroundWork* thisInstance = jobInstance->instance;

		if(thisInstance) {
			jobInstance->result = (thisInstance->instance->*thisInstance->workCallback)(jobInstance->savedArg);
		}
	}

	static void onAfterWork(uv_work_t* req, int status) {
		JobInstance* jobInstance = static_cast<JobInstance*>(req->data);
		BackgroundWork* thisInstance = jobInstance->instance;

		if(thisInstance && status == 0) {
			Argument savedArg = std::move(jobInstance->savedArg);
			int result = jobInstance->result;
			jobInstance->instance = nullptr;
			jobInstance->work.data = nullptr;

			thisInstance->ongoingJobs.remove(jobInstance);
			thisInstance->freeJobs.emplace_back(jobInstance);

			(thisInstance->instance->*thisInstance->afterWorkCallback)(std::move(savedArg), result);
		} else {
			delete jobInstance;
		}
	}

private:
	CallbackClass* instance;
	WorkCallback workCallback;
	AfterWorkCallback afterWorkCallback;

	struct JobInstance {
		BackgroundWork* instance;
		Argument savedArg;
		int result;

		uv_work_t work;
	};
	std::vector<std::unique_ptr<JobInstance>> freeJobs;
	std::list<JobInstance*> ongoingJobs;
};
