#ifndef TIMER_H
#define TIMER_H

#include "Extern.h"
#include <vector>
#include "uv.h"

class RZU_EXTERN Timer
{
public:
	Timer();
	~Timer();

	int start(uv_timer_cb cb, uint64_t timeout, uint64_t repeat);
	int stop();
	int again();
	void setRepeat(uint64_t repeat);
	uint64_t getRepeat() const;

private:
	uv_timer_t* handle;

	static void onClosedCallback(uv_handle_t* handle);

	static std::vector<uv_timer_t*> freeTimers;
};

#endif // TIMER_H
