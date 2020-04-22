#pragma once

#include <stdint.h>

class IDbQueryJob {
public:
	enum Status { S_Ok, S_Canceled, S_Error };

public:
	virtual void cancel() = 0;
	virtual ~IDbQueryJob() {}

public:  // events
	// To implement in derived classes
	// In a thread job
	virtual bool onPreProcess() { return true; }  // if return false, don't execute the rest (nor the DB query)
	virtual bool onRowDone() { return true; }
	virtual void onPostProcess() {}

	// In main thread
	virtual void onDone(Status status) {}

private:
	friend class DbQueryBinding;
	virtual void* getInputPointer(size_t i) = 0;
	virtual size_t getInputSize() = 0;
	virtual size_t getInputNumber() = 0;
	virtual void* createNextLineInstance() = 0;
};

