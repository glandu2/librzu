#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Object.h"
#include "uv.h"
#include <list>

class RZU_EXTERN EventLoop : public Object {
	DECLARE_CLASS(EventLoop)

public:
	EventLoop();
	~EventLoop();

	void addObjectToDelete(Object* o) { objectsToDelete.push_back(o); }
	void deleteObjects();

	void run(uv_run_mode mode) { uv_run(&loop, mode); }

	// one different loop per thread
	static EventLoop* getInstance();
	static uv_loop_t* getLoop() { return &getInstance()->loop; }

protected:
	static void staticDeleteObjects(uv_prepare_t* handle);

private:
	uv_loop_t loop;
	uv_prepare_t deleteObjectsHandle;
	std::list<Object*> objectsToDelete;

	static uv_once_t tlsKeyInitOnce;
	static uv_key_t tlsKey;
	static void initKey();
};

#endif  // EVENTLOOP_H
