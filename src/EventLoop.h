#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Object.h"
#include "uv.h"
#include <list>

class EventLoop : public Object
{
	DECLARE_CLASS(EventLoop)

public:
	EventLoop();

	void addObjectToDelete(Object* o) { objectsToDelete.push_back(o); }

	void run(uv_run_mode mode) { uv_run(loop, mode); }

	static EventLoop* getInstance();
	static uv_loop_t* getLoop() { return getInstance()->loop; }

protected:
	static void deleteObjects(uv_prepare_t* handle, int status);

private:
	uv_loop_t* loop;
	uv_prepare_t deleteObjectsHandle;
	std::list<Object*> objectsToDelete;
};

#endif // EVENTLOOP_H
