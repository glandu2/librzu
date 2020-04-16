#ifndef UVHANDLE_H
#define UVHANDLE_H

#include "EventLoop.h"
#include "Extern.h"
#include "uv.h"

template<typename HandleType> class RZU_EXTERN UvHandle {
public:
	HandleType* handle;
	template<typename Ret, typename... Args> UvHandle(Ret (*InitFunction)(Args... args)) {
		handle = get();
		InitFunction(EventLoop::getLoop(), handle);
	}
	~UvHandle();

	operator HandleType*() { return handle; }
	operator uv_handle_t*() { return (uv_handle_t*) handle; }
	operator const HandleType*() const { return handle; }
	operator const uv_handle_t*() const { return (uv_handle_t*) handle; }

	HandleType* operator->() { return handle; }
	const uv_handle_t* operator->() const { return (uv_handle_t*) handle; }

private:
	UvHandle(UvHandle&) = delete;
	UvHandle& operator=(UvHandle&) = delete;

	static HandleType* get();
	static void closeCallback(uv_handle_t* handle) { freeHandles.emplace_back((HandleType*) handle); }

	static std::vector<std::unique_ptr<HandleType>> freeHandles;
};

#endif
