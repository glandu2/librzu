#ifndef DBQUERYJOB_H
#define DBQUERYJOB_H

#include "Core/Object.h"
#include "uv.h"
#include "Core/EventLoop.h"

#include "DbQueryBinding.h"

class IDbQueryJob {
public:
	enum Status {
		S_Ok,
		S_Canceled,
		S_Error
	};

public:
	virtual void cancel() = 0;

public: //events
	//To implement in derived classes
	//In a thread job
	virtual bool onPreProcess() { return true; } //if return false, don't execute the rest (nor the DB query)
	virtual bool onRowDone() { return true; }
	virtual void onPostProcess() {}

	//In main thread
	virtual void onDone(Status status) {}

private:
	friend class DbQueryBinding;
	virtual void* createNextLineInstance() = 0;
};

template<class DbMappingClass>
class DbQueryJob : public Object, public IDbQueryJob {
public:
	typedef typename DbMappingClass::Input InputType;
	typedef typename DbMappingClass::Output OutputType;

	static bool init(DbConnectionPool* dbConnectionPool);
	static void deinit() {
		DbQueryBinding* binding = dbBinding;
		dbBinding = nullptr;
		delete binding;
	}

	static void executeNoResult(const InputType& input) {
		auto query = new DbQueryJob;
		query->execute(input);
	}

	DbQueryJob() : done(false), canceled(false) {}

	bool execute(const InputType& input) {
		done = false;
		canceled = false;

		if(dbBinding == nullptr) {
			error("DB binding was not initialized ! Canceling DB query\n");
			onDone(S_Canceled);
			return false;
		}

		this->input = input;
		req.data = this;
		uv_queue_work(EventLoop::getLoop(), &req, &onProcessStatic, &onDoneStatic);

		return true;
	}

	void cancel() {
		canceled = true;
		uv_cancel((uv_req_t*)&req);
	}

	bool isDone() { return done; }

	std::vector<OutputType>& getResults() { return outputLines; }
	InputType* getInput() { return &input; }

protected:
	static void onProcessStatic(uv_work_t *req) {
		DbQueryJob* dbQueryJob = (DbQueryJob*) req->data;
		dbQueryJob->onProcess();
	}

	void onProcess() {
		DbQueryBinding* binding = dbBinding;
		outputLines.clear();

		//check enabled here so onDone is called
		if(!binding || canceled) {
			done = false;
			return;
		}
		if(onPreProcess() == false) {
			debug("Canceled DB query in preprocess step\n");
			return;
		}
		done = binding->process(this, &input);

		onPostProcess();
	}

	static void onDoneStatic(uv_work_t *req, int status) {
		DbQueryJob* dbQueryJob = (DbQueryJob*) req->data;
		if(status == UV_ECANCELED)
			dbQueryJob->onDone(S_Canceled);
		else if(status == 0 && dbQueryJob->done == true)
			dbQueryJob->onDone(S_Ok);
		else
			dbQueryJob->onDone(S_Error);

		delete dbQueryJob;
	}

protected:
	virtual void* createNextLineInstance() {
		size_t size = outputLines.size();
		outputLines.resize(size + 1);
		return &outputLines[size];
	}

protected:
	~DbQueryJob() {}

private:
	uv_work_t req;
	InputType input;
	std::vector<OutputType> outputLines;
	bool done;
	volatile bool canceled;

	static DbQueryBinding* dbBinding;
};

#endif // DBQUERYJOB_H
